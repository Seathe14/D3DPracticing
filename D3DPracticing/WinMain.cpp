#include <sstream>

#include "WinHeader.h"
#include "App.h"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	try {
		return App{}.RunLoop();
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