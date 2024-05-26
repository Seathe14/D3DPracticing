#pragma once
#include "WinHeader.h"
#include <queue>
#include <bitset>
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
	void OnChar(WPARAM wParam);
	std::optional<Event> ReadKey();
	std::optional<char> ReadChar();

	void ClearState();
	void EnableAutoRepeat();
	void DisableAutoRepeat();
	void FlushKeyBuffer();
	void FlushCharBuffer();
	void Flush();

	bool IsAutoRepeatEnabled() const;
	bool IsKeyPressed(WPARAM wParam) const;
	bool KeyBufferIsEmpty() const;
	bool CharBufferIsEmpty() const;
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool isAutoRepeat{ false };
	std::queue<Event> keyBuffer;
	std::queue<char> charBuffer;
	std::bitset<nKeys> keyStates;

	template<typename T>
	static void TrimBuffer(std::queue<T>& queue);
};

template <typename T>
void Keyboard::TrimBuffer(std::queue<T>& queue)
{
	while (queue.size() > bufferSize)
	{
		queue.pop();
	}
}
