#include "Mouse.h"

static std::tuple<int, int> ExtractCoords(LPARAM lParam)
{
	const POINTS pt = MAKEPOINTS(lParam);
	return { pt.x, pt.y };
}

void Mouse::OnLeftPressed(WPARAM wParam, LPARAM lParam)
{
	leftIsPressed = true;
	OnEventImpl(Event::Type::LPress, lParam);
}

void Mouse::OnLeftReleased(WPARAM wParam, LPARAM lParam)
{
	leftIsPressed = false;
	OnEventImpl(Event::Type::LRelease, lParam);
}

void Mouse::OnRightPressed(WPARAM wParam, LPARAM lParam)
{
	rightIsPressed = true;
	OnEventImpl(Event::Type::RPress, lParam);
}

void Mouse::OnRightReleased(WPARAM wParam, LPARAM lParam)
{
	rightIsPressed = false;
	OnEventImpl(Event::Type::RRelease, lParam);
}

void Mouse::OnMiddlePressed(WPARAM wParam, LPARAM lParam)
{
	OnEventImpl(Event::Type::MPress, lParam);
}

void Mouse::OnMiddleReleased(WPARAM wParam, LPARAM lParam)
{
	OnEventImpl(Event::Type::MRelease, lParam);
}

void Mouse::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	OnEventImpl(Event::Type::Move, lParam);
}

void Mouse::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
	wheelDeltaCarry += delta;
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnEventImpl(Event::Type::WheelUp, lParam);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnEventImpl(Event::Type::WheelDown, lParam);
	}
}

void Mouse::OnMouseEnter()
{
	isInWindow = true;
	OnEventImpl(Event::Type::Enter);
}

void Mouse::OnMouseLeave()
{
	isInWindow = false;
	OnEventImpl(Event::Type::Leave);
}

void Mouse::Flush()
{
	std::queue<Event>().swap(mouseBuffer);
}

std::optional<Mouse::Event> Mouse::Read() const
{
	if (!IsEmpty())
	{
		Event ev = mouseBuffer.front();
		mouseBuffer.pop();
		return ev;
	}
	return std::nullopt;
}

void Mouse::OnEventImpl(Event::Type type, std::optional<LPARAM> lParam)
{
	auto f = [this, type](int x, int y) {
		this->x = x;
		this->y = y;
		mouseBuffer.emplace(type, x, y, leftIsPressed, rightIsPressed);
	};
	if (lParam.has_value()) {
		std::apply(f, ExtractCoords(*lParam));
	}
	else {
		f(x, y);
	}
	TrimBuffer();
}
