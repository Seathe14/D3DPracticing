#include <sstream>

#include "WinHeader.h"
#include "App.h"

int WINAPI wWinMain([[maybe_unused]] _In_ HINSTANCE hInstance
				  , [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance
				  , [[maybe_unused]] _In_ PWSTR pCmdLine
				  , [[maybe_unused]] _In_ int nCmdShow)
{
	try {
		return App{}.RunLoop();
	}
	catch(const BaseException& e)
	{
		MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION | MB_DEFAULT_DESKTOP_ONLY);
	}
	catch(const std::exception& e)
	{
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION | MB_DEFAULT_DESKTOP_ONLY);
	}
	catch(...)
	{
		MessageBox(nullptr, "Unknown exception", "Unknown exception", MB_OK | MB_ICONEXCLAMATION | MB_DEFAULT_DESKTOP_ONLY);
	}
	return -1;
}