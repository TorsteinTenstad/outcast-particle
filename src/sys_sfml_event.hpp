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

			for (auto& [_, pressed_this_frame] : cursor_and_keys.key_pressed_this_frame)
			{
				(void)_;
				pressed_this_frame = false;
			}
			for (auto& [_, released_this_frame] : cursor_and_keys.key_released_this_frame)
			{
				(void)_;
				released_this_frame = false;
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
			auto mouse_pos = sf::Mouse::getPosition(globals.render_window);
			cursor_and_keys.cursor_position.x = mouse_pos.x;
			cursor_and_keys.cursor_position.y = mouse_pos.y;
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					cursor_and_keys.left_button_is_pressed = true;
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					cursor_and_keys.right_button_is_pressed = true;
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					cursor_and_keys.left_button_is_pressed = false;
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					cursor_and_keys.right_button_is_pressed = false;
				}
			}
		}
	}
};