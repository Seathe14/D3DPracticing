#pragma once
#include <queue>

#include "WinHeader.h"

class Mouse
{
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	Mouse(Mouse&&) = delete;
	Mouse& operator=(Mouse&&) = delete;
	~Mouse() = default;
	struct Pos
	{
		int x;
		int y;
	};
	class Event
	{
	public:
		enum class Type;
		Event() = delete;
		Event(Type type, int xCoord, int yCoord, bool leftIsPressed, bool rightIsPressed)
		: type(type)
		, pos(xCoord, yCoord)
		, leftIsPressed(leftIsPressed)
		, rightIsPressed(rightIsPressed) {}

		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			MPress,
			MRelease,
			Move,
			Enter,
			Leave,
			Invalid
		};
		Type GetType() const
		{
			return type;
		}
		Pos GetPos() const
		{
			return pos;
		}
		bool IsLeftPressed() const
		{
			return leftIsPressed;
		}
		bool IsRightPressed() const
		{
			return rightIsPressed;
		}
	private:
		Type type{ Type::Invalid };
		Pos pos;
		bool leftIsPressed{ false };
		bool rightIsPressed{ false };
	};

	void OnLeftPressed(WPARAM wParam, LPARAM lParam);
	void OnLeftReleased(WPARAM wParam, LPARAM lParam);
	void OnRightPressed(WPARAM wParam, LPARAM lParam);
	void OnRightReleased(WPARAM wParam, LPARAM lParam);
	void OnMiddlePressed(WPARAM wParam, LPARAM lParam);
	void OnMiddleReleased(WPARAM wParam, LPARAM lParam);
	void OnMouseMove(WPARAM wParam, LPARAM lParam);
	void OnMouseWheel(WPARAM wParam, LPARAM lParam);
	void OnMouseEnter();
	void OnMouseLeave();

	void Flush();
	std::optional<Event> Read() const;

	Pos GetPos() const
	{
		return { x,y };
	}
	bool IsLeftPressed() const
	{
		return leftIsPressed;
	}
	bool IsRightPressed() const
	{
		return rightIsPressed;
	}
	bool IsEmpty() const
	{
		return mouseBuffer.empty();
	}
	bool IsInWindow() const
	{
		return isInWindow;
	}

private:
	static constexpr unsigned int bufferSize = 16u;
	int x{ 0 };
	int y{ 0 };
	int wheelDeltaCarry{ 0 };
	bool leftIsPressed{ false };
	bool rightIsPressed{ false };
	bool isInWindow{ false };
	mutable std::queue<Event> mouseBuffer;

	void OnEventImpl(Event::Type type, std::optional<LPARAM> lParam = std::nullopt);

	void TrimBuffer()
	{
		while (mouseBuffer.size() > bufferSize)
		{
			mouseBuffer.pop();
		}
	}
};