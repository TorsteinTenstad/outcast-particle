#pragma once

#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2i WorldPosToGridPos(sf::Vector2f world_pos, float grid_size)
{
	sf::Vector2f unrounded_grid_pos = world_pos / grid_size;
	return sf::Vector2i((int)std::floor(unrounded_grid_pos.x), (int)std::floor(unrounded_grid_pos.y));
}

class GridPositionSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		GridPositionManager* grid_position_manager = GetSingleton<GridPositionManager>(level);

		sf::Vector2i last_frame_cursor_grid_position = WorldPosToGridPos(cursor_and_keys_.last_frame_cursor_position, BLOCK_SIZE);
		sf::Vector2i cursor_grid_position = WorldPosToGridPos(cursor_and_keys_.cursor_position, BLOCK_SIZE);
		sf::Vector2i mouse_button_last_pressed_grid_position = WorldPosToGridPos(cursor_and_keys_.mouse_button_last_pressed_position[sf::Mouse::Button::Left], BLOCK_SIZE);
		if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Button::Left] || (cursor_and_keys_.mouse_button_down[sf::Mouse::Button::Left] && last_frame_cursor_grid_position != cursor_grid_position))
		{
			level.DeleteEntitiesWith<EditModeTemporary>();
			level.GetComponent<TemporarilySelected>().clear();
			int i_incr = Sign(cursor_grid_position.x - mouse_button_last_pressed_grid_position.x);
			for (int i = mouse_button_last_pressed_grid_position.x; i != cursor_grid_position.x + i_incr; i += i_incr)
			{
				int j_incr = Sign(cursor_grid_position.y - mouse_button_last_pressed_grid_position.y);
				for (int j = mouse_button_last_pressed_grid_position.y; j != cursor_grid_position.y + j_incr; j += j_incr)
				{
					if (cursor_and_keys_.key_down[sf::Keyboard::Num1])
					{
						int id = level.AddBlueprint("BPWall");
						auto cursor_grid_pos = WorldPosToGridPos(cursor_and_keys_.cursor_position, BLOCK_SIZE);
						level.GetComponent<GridPosition>()[id].grid_position = sf::Vector2i(i, j);
						level.AddComponents<EditModeTemporary>(id);
						grid_position_manager->dirty = true;
					}
					else if (cursor_and_keys_.key_down[sf::Keyboard::Num2])
					{
						int id = level.AddBlueprint("BPBounceWall");
						auto cursor_grid_pos = WorldPosToGridPos(cursor_and_keys_.cursor_position, BLOCK_SIZE);
						level.GetComponent<GridPosition>()[id].grid_position = sf::Vector2i(i, j);
						level.AddComponents<EditModeTemporary>(id);
						grid_position_manager->dirty = true;
					}
					else
					{
						if (grid_position_manager->entity_id_at_position.count({ i, j }))
						{
							level.AddComponent<TemporarilySelected>(grid_position_manager->entity_id_at_position.at({ i, j }));
						}
					}
				}
			}
		}
		if (cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Button::Left])
		{
			for (auto [entity_id, temporarity_selected] : level.GetEntitiesWith<TemporarilySelected>())
			{
				level.AddComponent<Selected>(entity_id);
			}
			level.GetComponent<TemporarilySelected>().clear();
			level.GetComponent<EditModeTemporary>().clear();
			for (auto [entity_id, grid_position] : level.GetEntitiesWith<GridPosition>())
			{
				auto grid_index = Vector2iToArray(grid_position->grid_position);
				if (grid_position_manager->entity_id_at_position.at(grid_index) != entity_id)
				{
					level.DeleteEntity(entity_id);
				}
			}
		}
		/*
		if (level.GetMode() == EDIT_MODE && cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Button::Right])
		{
			auto cursor_grid_pos = WorldPosToGridPos(cursor_and_keys_.cursor_position, BLOCK_SIZE);
			auto grid_index = Vector2iToArray(cursor_grid_pos);
			level.DeleteEntity(grid_position_manager->entity_id_at_position[grid_index]);
			grid_position_manager->dirty = true;
		}*/

		if (!grid_position_manager->dirty)
		{
			return;
		}
		grid_position_manager->dirty = false;
		for (auto [entity_id, grid_position, position] : level.GetEntitiesWith<GridPosition, Position>())
		{
			position->position = (sf::Vector2f(grid_position->grid_position) + sf::Vector2f(0.5, 0.5)) * float(BLOCK_SIZE);
			auto grid_index = Vector2iToArray(grid_position->grid_position);
			//assert(grid_position_manager->entity_id_at_position.count(grid_index) == 0);
			grid_position_manager->entity_id_at_position[grid_index] = entity_id;
		}
	}
};