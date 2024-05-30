#include "Window.h"
#include "Graphics.h"
#include "WindowsMacros.h"
#include <sstream>

Window::WindowPrivate::WindowPrivate() : hInst(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}

Window::WindowPrivate::~WindowPrivate()
{
	UnregisterClass(GetName(), GetInstance());
}

LRESULT Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk)); 
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN: [[fallthrough]];
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.IsAutoRepeatEnabled()) 
		{
			kbd.OnKeyPressed(wParam);
		}
		break;
	case WM_KEYUP: [[fallthrough]];
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(wParam);
		break;
	case WM_LBUTTONDOWN:
		mouse.OnLeftPressed(wParam, lParam);
		break;
	case WM_LBUTTONUP:
		mouse.OnLeftReleased(wParam, lParam);
		break;
	case WM_RBUTTONDOWN:
		mouse.OnRightPressed(wParam, lParam);
		break;
	case WM_RBUTTONUP:
		mouse.OnRightReleased(wParam, lParam);
		break;
	case WM_MOUSEWHEEL:
		mouse.OnMouseWheel(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(wParam, lParam);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (mouse.IsLeftPressed() || mouse.IsRightPressed())
			{
				mouse.OnMouseMove(wParam, lParam);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	case WM_CHAR:
		kbd.OnChar(wParam);
		break;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

const char* Window::WindowPrivate::GetName()
{
	return wndClassName.data();
}

HINSTANCE Window::WindowPrivate::GetInstance()
{
	return GetWndClass().hInst;
}

Window::WindowPrivate& Window::WindowPrivate::GetWndClass()
{
	static WindowPrivate wndClass;
	return wndClass;
}


Window::Exception::Exception(int line, std::string_view file, HRESULT hr)
	: BaseException(line, file)
	, hr(hr)
{
}

const char* Window::Exception::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << static_cast<unsigned long>(GetErrorCode()) << ")" << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Window::Exception::GetType() const
{
	return "Window exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
								  nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const
{
	return hr;
}

std::string Window::Exception::GetErrorString() const
{
	return TranslateErrorCode(GetErrorCode());
}

Window::Window(int width, int height, std::string_view name)
			  : width(width)
			  , height(height)
{
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (!AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
	{
		throw WND_LAST_EXCEPT();
	}
	hWnd = CreateWindow(WindowPrivate::GetName(), name.data(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, nullptr,
						nullptr, WindowPrivate::GetInstance(), this);
	if (hWnd == nullptr)
	{
		throw WND_LAST_EXCEPT();
	}
	ShowWindow(hWnd, SW_SHOWDEFAULT);

	pGfx = std::make_unique<Graphics>(hWnd);
}

Window::~Window()
{
	//DestroyWindow(hWnd);
}

void Window::SetTitle(std::string_view title)
{
	if (!SetWindowText(hWnd, title.data()))
	{
		throw WND_LAST_EXCEPT();
	}
}

const Keyboard& Window::GetKeyboard() const
{
	return kbd;
}

const Mouse& Window::GetMouse() const
{
	return mouse;
}
Graphics& Window::GetGfx()
{
	return *pGfx;
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT) {
			return static_cast<int>(msg.wParam);
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return std::nullopt;
}
