#include "Keyboard.h"

#include <sstream>

void Keyboard::OnKeyPressed(WPARAM wParam)
{
	keyStates[wParam] = true;
	keyBuffer.emplace(Event::Type::Press, wParam);
	TrimBuffer(keyBuffer);
}

void Keyboard::OnKeyReleased(WPARAM wParam)
{
	keyStates[wParam] = false;
	keyBuffer.emplace(Event::Type::Release, wParam);
	TrimBuffer(keyBuffer);
}

void Keyboard::OnChar(WPARAM wParam)
{
	charBuffer.push(static_cast<char>(wParam));
	TrimBuffer(charBuffer);
}

std::optional<Keyboard::Event> Keyboard::ReadKey()
{
	if (!KeyBufferIsEmpty())
	{
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
	return std::nullopt;
}

std::optional<char> Keyboard::ReadChar()
{
	if (!CharBufferIsEmpty())
	{
		char ret = charBuffer.front();
		charBuffer.pop();
		return ret;
	}
	return std::nullopt;
}

void Keyboard::ClearState()
{
	keyStates.reset();
}

void Keyboard::EnableAutoRepeat()
{
	isAutoRepeat = true;
}

void Keyboard::DisableAutoRepeat()
{
	isAutoRepeat = false;
}

void Keyboard::FlushKeyBuffer()
{
	std::queue<Event>().swap(keyBuffer);
}

void Keyboard::FlushCharBuffer()
{
	std::queue<char>().swap(charBuffer);
}

void Keyboard::Flush()
{
	FlushKeyBuffer();
	FlushCharBuffer();
}

bool Keyboard::IsAutoRepeatEnabled() const
{
	return isAutoRepeat;
}

bool Keyboard::IsKeyPressed(WPARAM wParam) const
{
	return keyStates[wParam];
}

bool Keyboard::KeyBufferIsEmpty() const
{
	return keyBuffer.empty();
}

bool Keyboard::CharBufferIsEmpty() const
{
	return charBuffer.empty();
}
