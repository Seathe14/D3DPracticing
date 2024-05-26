#include "WinHeader.h"
#include "Window.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	try {
		Window wnd(400, 400, "Brakba");
		MSG msg;
		BOOL gResult;
		while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (wnd.GetKeyboard().IsKeyPressed(VK_MENU))
			{
				MessageBox(nullptr, "something happen", "the alt key was pressed", MB_OK);
			}
		}
		if (gResult == -1)
			return -1;
		return static_cast<int>(msg.wParam);
	}
	catch(const BaseException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch(const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch(...)
	{
		MessageBox(nullptr, "Unknown exception", "Unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}