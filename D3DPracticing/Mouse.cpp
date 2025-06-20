#include "Mouse.h"

#include <sstream>

static std::tuple<int, int> ExtractCoords(LPARAM lParam)
{
	const POINTS pt = MAKEPOINTS(lParam);

	return { pt.x, pt.y };
}

void Mouse::OnLeftPressed([[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
	leftIsPressed = true;
	OnEventImpl(Event::Type::LPress, lParam);
}

void Mouse::OnLeftReleased([[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
	leftIsPressed = false;
	OnEventImpl(Event::Type::LRelease, lParam);
}

void Mouse::OnRightPressed([[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
	rightIsPressed = true;
	OnEventImpl(Event::Type::RPress, lParam);
}

void Mouse::OnRightReleased([[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
	rightIsPressed = false;
	OnEventImpl(Event::Type::RRelease, lParam);
}

void Mouse::OnMiddlePressed([[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
	OnEventImpl(Event::Type::MPress, lParam);
}

void Mouse::OnMiddleReleased([[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
	OnEventImpl(Event::Type::MRelease, lParam);
}

void Mouse::OnMouseMove([[maybe_unused]] WPARAM wParam, LPARAM lParam)
{
	OnEventImpl(Event::Type::Move, lParam);
}

void Mouse::OnMouseWheel([[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
	const auto delta = GET_WHEEL_DELTA_WPARAM(wParam);
	wheelDeltaCarry += delta;

	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnEventImpl(Event::Type::WheelUp);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnEventImpl(Event::Type::WheelDown);
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

void Mouse::Flush() const
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
