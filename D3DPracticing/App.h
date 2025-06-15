#pragma once

#include "Timer.h"
#include "Window.h"
#include "Drawable.h"

class App
{
public:
	App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;
	App(App&&) = delete;
	App& operator=(App&&) = delete;
	~App();

	int RunLoop();

protected:
	void DoFrame();
	Window wnd;
	Timer tmr;
        std::vector<std::unique_ptr<Drawable>> drawables;
};
