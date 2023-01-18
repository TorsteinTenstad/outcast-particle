#pragma once

#include "game_system.hpp"
#include "level.hpp"
#include <any>

class MenuNavigatonSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, menu_navigator, width_and_height, position] : level.GetEntitiesWith<MenuNavigator, WidthAndHeight, Position>())
		{
			if (menu_navigator->currently_at_entity_id < 0
				|| cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Down]
				|| cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Up])
			{
				int attatch_to_id = -1;
				std::map<int, sf::Vector2f> possble_positions = GetPossiblePossiblePositions(level);

				if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Up])
				{
					attatch_to_id = possble_positions.rbegin()->first;
					for (auto it = possble_positions.rbegin(); it != possble_positions.rend(); ++it)
					{
						if (it->first < menu_navigator->currently_at_entity_id)
						{
							attatch_to_id = it->first;
							break;
						}
					}
				}
				else
				{
					attatch_to_id = possble_positions.begin()->first;
					for (auto it = possble_positions.begin(); it != possble_positions.end(); ++it)
					{
						if (it->first > menu_navigator->currently_at_entity_id)
						{
							attatch_to_id = it->first;
							break;
						}
					}
				}
				assert(attatch_to_id >= 0);
				menu_navigator->currently_at_entity_id = attatch_to_id;
				position->position = possble_positions[attatch_to_id];
				position->position.x -= width_and_height->width_and_height.x;
			}
			assert(menu_navigator->currently_at_entity_id >= 0);
			if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Enter])
			{
				level.GetComponent<PressedThisFrame>()[menu_navigator->currently_at_entity_id];
			}
			if (cursor_and_keys_.key_released_this_frame[sf::Keyboard::Enter])
			{
				level.GetComponent<ReleasedThisFrame>()[menu_navigator->currently_at_entity_id];
			}
		}
	}

	std::map<int, sf::Vector2f> GetPossiblePossiblePositions(Level& level)
	{
		std::map<int, sf::Vector2f> possible_positions;
		for (auto [entity_id, menu_navigator, width_and_height, position] : level.GetEntitiesWith<MenuNavigatable, WidthAndHeight, Position>())
		{
			possible_positions[entity_id] = position->position;
			possible_positions[entity_id].x -= width_and_height->width_and_height.x / 2;
		}
		assert(possible_positions.size() > 0);
		return possible_positions;
	}
};