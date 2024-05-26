#include "App.h"

App::App() : wnd(800,600, "myWnd")
{
}

int App::RunLoop()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
	}
}

void App::DoFrame()
{

}
