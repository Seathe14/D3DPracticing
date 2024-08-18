#include "App.h"

#include <random>

#include "Box.h"
#include "Graphics.h"

App::App() : wnd(800,600, "myWnd")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 15.0f);
	for (auto i = 0; i < 120; i++)
	{
		boxes.push_back(std::make_unique<Box>(
			wnd.GetGfx(), rng, adist,
			ddist, odist, rdist
		));
	}
	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

App::~App() = default;

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
	//const float c = std::sin(tmr.Peek()) / 2.0f + 0.5f;
	//wnd.GetGfx().ClearBuffer(c, c, 1.0f);
	//Mouse::Pos pos = wnd.GetMouse().GetPos();
	//wnd.GetGfx().DrawTestTriangle(tmr.Peek(), 0, 0);
	//wnd.GetGfx().DrawTestTriangle(tmr.Peek(), static_cast<float>(pos.x) / 400.f - 1.0f, static_cast<float>(-pos.y) / 300.f + 1.0f);
	//wnd.GetGfx().EndFrame();
	auto dt = tmr.Mark();
	wnd.GetGfx().ClearBuffer(0.07f, 0.0f, 0.12f);

	for (auto& b : boxes)
	{
		b->Update(dt);
		b->Draw(wnd.GetGfx());
	}

	wnd.GetGfx().EndFrame();
}
