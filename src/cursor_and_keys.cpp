#pragma once
#include "cursor_and_keys.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/window/Keyboard.hpp"
#include "SFML/window/Mouse.hpp"
#include <map>

CursorAndKeys::CursorAndKeys()
{
	for (int key = 0; key < sf::Keyboard::KeyCount; key++)
	{
		key_down[key] = false;
		key_pressed_this_frame[key] = false;
		key_released_this_frame[key] = false;
	}
	for (int button = 0; button < sf::Mouse::ButtonCount; button++)
	{
		mouse_button_down[button] = false;
		mouse_button_pressed_this_frame[button] = false;
		mouse_button_released_this_frame[button] = false;
	}
}
void CursorAndKeys::ResetFrameEvents()
{
	mouse_wheel_delta = 0;
	for (int key = 0; key < sf::Keyboard::KeyCount; key++)
	{
		key_pressed_this_frame[key] = false;
		key_released_this_frame[key] = false;
	}
	for (int button = 0; button < sf::Mouse::ButtonCount; button++)
	{
		mouse_button_pressed_this_frame[button] = false;
		mouse_button_released_this_frame[button] = false;
	}
	text_input.clear();
}

void CursorAndKeys::ResetAllKeysAndButtons()
{
	ResetFrameEvents();
	for (int key = 0; key < sf::Keyboard::KeyCount; key++)
	{
		key_down[key] = false;
	}
	for (int button = 0; button < sf::Mouse::ButtonCount; button++)
	{
		mouse_button_down[button] = false;
	}
}