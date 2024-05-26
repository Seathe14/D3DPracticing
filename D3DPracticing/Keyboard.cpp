#include "Keyboard.h"

void Keyboard::OnKeyPressed(WPARAM wParam)
{
	keyBuffer.emplace(Event::Type::Press, wParam);
}

void Keyboard::OnKeyReleased(WPARAM wParam)
{
	keyBuffer.emplace(Event::Type::Release, wParam);
}

bool Keyboard::KeybufferIsEmpty() const
{
	return keyBuffer.empty();
}
