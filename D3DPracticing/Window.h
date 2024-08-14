#pragma once
#include "WinHeader.h"
#include "BaseException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <memory>

class BaseException;
class Graphics;

class Window
{
public:
	class Exception : public BaseException
	{
	public:
		Exception(int line, std::string_view file, HRESULT hr);
		const char* what() const override;
		const char* GetType() const override;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const;
		std::string GetErrorString() const;
	private:
		HRESULT hr;
	};

	Window(int width, int height, std::string_view name);
	~Window();
	Window(const Window&) = delete;
	Window& operator= (const Window&) = delete;
	void SetTitle(std::string_view title) const;

	const Keyboard& GetKeyboard() const;
	const Mouse& GetMouse() const;
	Graphics& GetGfx() const;

	static std::optional<int> ProcessMessages();
private:
	class WindowPrivate
	{
	public:
		static const char* GetName();
		static HINSTANCE GetInstance();
	private:
		WindowPrivate();
		~WindowPrivate();
		WindowPrivate(const WindowPrivate&) = delete;
		WindowPrivate& operator=(const WindowPrivate&) = delete;
		static WindowPrivate& GetWndClass();

		static inline std::string_view wndClassName = "MyWindow";
		HINSTANCE hInst;
	};
	int width{ 0 };
	int height{ 0 };
	HWND hWnd{ NULL };
	Keyboard kbd;
	Mouse mouse;
	std::unique_ptr<Graphics> pGfx;

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND _hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};
