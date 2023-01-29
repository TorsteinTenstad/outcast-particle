#pragma once
#include "SFML/Window/Event.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class SFMLEventHandler
{
private:
public:
	void Update(CursorAndKeys& cursor_and_keys)
	{
		cursor_and_keys.ResetFrameEvents();
		sf::Event event;
		while (globals.render_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				globals.render_window.close();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code != sf::Keyboard::Unknown)
			{
				cursor_and_keys.key_pressed_this_frame[event.key.code] = !cursor_and_keys.key_down[event.key.code];
				cursor_and_keys.key_down[event.key.code] = true;
			}
			if (event.type == sf::Event::KeyReleased && event.key.code != sf::Keyboard::Unknown)
			{
				cursor_and_keys.key_released_this_frame[event.key.code] = true;
				cursor_and_keys.key_down[event.key.code] = false;
			}
			auto mouse_pos = globals.render_window.mapPixelToCoords(sf::Mouse::getPosition(globals.render_window));
			cursor_and_keys.cursor_position.x = mouse_pos.x;
			cursor_and_keys.cursor_position.y = mouse_pos.y;
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				cursor_and_keys.mouse_wheel_delta = event.mouseWheelScroll.delta;
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				cursor_and_keys.mouse_button_pressed_this_frame[event.mouseButton.button] = true;
				cursor_and_keys.mouse_button_down[event.mouseButton.button] = true;
				cursor_and_keys.mouse_button_last_pressed_position[event.mouseButton.button] = sf::Vector2f(mouse_pos.x, mouse_pos.y);
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				cursor_and_keys.mouse_button_released_this_frame[event.mouseButton.button] = true;
				cursor_and_keys.mouse_button_down[event.mouseButton.button] = false;
				cursor_and_keys.mouse_button_last_released_position[event.mouseButton.button] = sf::Vector2f(mouse_pos.x, mouse_pos.y);
			}
		}
	}
};