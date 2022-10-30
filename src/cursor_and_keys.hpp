#pragma once

class CursorAndKeys
{
public:
	sf::Vector2f cursor_position;
	bool right_button_is_pressed = false;
	bool left_button_is_pressed = false;

	std::map<int, bool> key_down;
	std::map<int, bool> key_pressed_this_frame;
	std::map<int, bool> key_released_this_frame;

	CursorAndKeys()
	{
		for (int key = sf::Keyboard::A; sf::Keyboard::A != sf::Keyboard::Pause; key++)
		{
			key_down[key] = false;
			key_pressed_this_frame[key] = false;
			key_released_this_frame[key] = false;
		}
	}
};