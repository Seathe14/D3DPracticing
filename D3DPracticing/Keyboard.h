#pragma once
#include "WinHeader.h"
#include <queue>

class Keyboard
{
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	Keyboard(Keyboard&&) = delete;
	Keyboard& operator=(Keyboard&&) = delete;
	~Keyboard() = default;

	class Event
	{
	public:
		enum class Type;
		Event() = default;
		Event(Type type, WPARAM code) : type(type), code(code) {};
		bool IsPress() const
		{
			return type == Type::Press;
		}
		bool IsRelease() const
		{
			return type == Type::Release;
		}
		WPARAM GetCode() const
		{
			return code;
		}

		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type{Type::Invalid};
		WPARAM code{0};
	};

	void OnKeyPressed(WPARAM wParam);
	void OnKeyReleased(WPARAM wParam);
	bool IsKeyPressed(WPARAM wParam) const;
	bool KeybufferIsEmpty() const;

private:
	std::queue<Event> keyBuffer;
};