#pragma once
#include "PCH.hpp"
#include "comp_physics.hpp"
#include "comp_player.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"

class SFMLEventSystem
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
			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				globals.render_window.setView(sf::View(visibleArea));
			}
			if (event.type == sf::Event::KeyPressed)
			{
				cursor_and_keys.key_pressed_this_frame[event.key.code] = true;
				cursor_and_keys.key_down[event.key.code] = true;
			}
			if (event.type == sf::Event::KeyReleased)
			{
				cursor_and_keys.key_released_this_frame[event.key.code] = true;
				cursor_and_keys.key_down[event.key.code] = false;
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				cursor_and_keys.mouse_button_pressed_this_frame[event.mouseButton.button] = true;
				cursor_and_keys.mouse_button_down[event.mouseButton.button] = true;
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				cursor_and_keys.mouse_button_released_this_frame[event.mouseButton.button] = true;
				cursor_and_keys.mouse_button_down[event.mouseButton.button] = false;
			}
			auto mouse_pos = sf::Mouse::getPosition(globals.render_window);
			cursor_and_keys.cursor_position.x = mouse_pos.x;
			cursor_and_keys.cursor_position.y = mouse_pos.y;
			if (event.type == sf::Event::MouseWheelScrolled)
			{
				cursor_and_keys.mouse_wheel_delta = event.mouseWheelScroll.delta;
			}
		}
	}
};