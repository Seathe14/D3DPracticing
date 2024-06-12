#include "App.h"
#include "Graphics.h"

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
		DoFrame();
	}
}

void App::DoFrame()
{
	const float c = std::sin(tmr.Peek()) / 2.0f + 0.5f;
	wnd.GetGfx().ClearBuffer(c, c, 1.0f);
	Mouse::Pos pos = wnd.GetMouse().GetPos();
	wnd.GetGfx().DrawTestTriangle(tmr.Peek(), (pos.x / 400.f) - 1.0f, (-pos.y / 300.f) + 1.0f);
	wnd.GetGfx().EndFrame();
}
