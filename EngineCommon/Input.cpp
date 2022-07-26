#include "pch.h"
#include "Input.h"

void ATLAT::Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	memset(&_keyState[0], static_cast<int>(KeyState::Idle), KeyCount);
}

void ATLAT::Input::Update()
{
	HWND hwnd = ::GetActiveWindow();

	if (hwnd != _hwnd)
	{
		for (size_t i = 0; i < KeyCount; i++)
			_keyState[i] = KeyState::Idle;
		return;
	}
	
	BYTE keyboardStates[KeyCount] = {};
	if (::GetKeyboardState(keyboardStates) == false)
		return;

	for (uint32_t i = 0; i < KeyCount; i++)
	{
		KeyState& state = _keyState[i];

		if (keyboardStates[i] & 0x80)
		{
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Press;
			else
				state = KeyState::Down;
		}
		else
		{
			if (state == KeyState::Idle || state == KeyState::Up)
				state = KeyState::Idle;
			else
				state = KeyState::Up;
		}
	}
}