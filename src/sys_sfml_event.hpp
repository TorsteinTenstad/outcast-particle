#pragma once
#include "PCH.hpp"
#include "comp_cursor.hpp"
#include "comp_physics.hpp"
#include "comp_player.hpp"
#include "globals.hpp"

class SFMLEventSystem
{
private:
public:
	void Update(Cursor& cursor, std::map<int, Player>& player_map)
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
			if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
			{
				for (auto& [_, player] : player_map)
				{
					(void)_;
					if (event.key.code == sf::Keyboard::W)
					{
						player.moving_up = (event.type == sf::Event::KeyPressed);
					}
					if (event.key.code == sf::Keyboard::A)
					{
						player.moving_left = (event.type == sf::Event::KeyPressed);
					}
					if (event.key.code == sf::Keyboard::S)
					{
						player.moving_down = (event.type == sf::Event::KeyPressed);
					}
					if (event.key.code == sf::Keyboard::D)
					{
						player.moving_right = (event.type == sf::Event::KeyPressed);
					}
				}
			}

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					globals.edit_mode = !globals.edit_mode;
				}
				if (event.key.code == sf::Keyboard::Num0)
				{
					globals.active_level = 0;
				}
				if (event.key.code == sf::Keyboard::Num1)
				{
					globals.active_level = 1;
				}
			}
			auto mouse_pos = sf::Mouse::getPosition(globals.render_window);
			cursor.position.x = mouse_pos.x;
			cursor.position.y = mouse_pos.y;
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					cursor.left_button_is_pressed = true;
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					cursor.right_button_is_pressed = true;
				}
			}

			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					cursor.left_button_is_pressed = false;
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					cursor.right_button_is_pressed = false;
				}
			}
		}
	}
};