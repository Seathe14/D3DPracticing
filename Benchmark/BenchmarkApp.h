#include "App.h"
#include "Graphics.h"
class BenchmarkApp : private App
{
public:
	Window& GetWnd()
	{
		return wnd;
	}
	void DrawTestTriangles()
	{
		const float c = std::sin(tmr.Peek()) / 2.0f + 0.5f;
		wnd.GetGfx().ClearBuffer(c, c, 1.0f);
		Mouse::Pos pos = wnd.GetMouse().GetPos();
		wnd.GetGfx().DrawTestTriangle(tmr.Peek(), 0, 0);
	/*	for (int i = 0; i < 30; i++)
		{
			wnd.GetGfx().DrawTestTriangle(tmr.Peek(), 0, 0);
		}*/
		wnd.GetGfx().DrawTestTriangle(tmr.Peek(), static_cast<float>(pos.x) / 400.f - 1.0f, static_cast<float>(-pos.y) / 300.f + 1.0f);
		wnd.GetGfx().EndFrame();
	}
};