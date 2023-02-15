#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/window/Mouse.hpp"
#include <map>

class CursorAndKeys
{
public:
	float mouse_wheel_delta = 0;

	sf::Vector2f cursor_position;
	sf::Vector2i screen_space_cursor_position;
	bool cursor_moved_this_frame = false;
	sf::Vector2f last_frame_cursor_position;
	sf::Vector2i last_frame_screen_space_cursor_position;
	std::map<int, sf::Vector2f> mouse_button_last_pressed_position;
	std::map<int, sf::Vector2f> mouse_button_last_released_position;
	std::map<int, bool> mouse_button_down;
	std::map<int, bool> mouse_button_pressed_this_frame;
	std::map<int, bool> mouse_button_released_this_frame;

	std::map<int, bool> key_down;
	std::map<int, bool> key_pressed_this_frame;
	std::map<int, bool> key_released_this_frame;

	CursorAndKeys()
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
	void ResetFrameEvents()
	{
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
	}
};