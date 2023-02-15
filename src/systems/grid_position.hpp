#pragma once

#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2i WorldPosToGridPos(sf::Vector2f world_pos, float grid_size, sf::Vector2u max_grid_size)
{
	sf::Vector2f unrounded_grid_pos = world_pos / grid_size;
	return sf::Vector2i(
		Clamp((int)std::floor(unrounded_grid_pos.x), (int)0, (int)max_grid_size.x - 1),
		Clamp((int)std::floor(unrounded_grid_pos.y), (int)0, (int)max_grid_size.y - 1));
}

static void TemporarilySelectEntity(Level& level, int entity_id)
{
	level.AddComponent<TemporarilySelected>(entity_id);
	level.AddComponent<FillColor>(entity_id)->color = sf::Color(150, 150, 255);
}

static void SelectEntity(Level& level, int entity_id)
{
	level.AddComponent<Selected>(entity_id);
	level.AddComponent<FillColor>(entity_id)->color = sf::Color(150, 150, 255);
}

template <class SelectedComponentVariant>
static void DeselectAll(Level& level)
{
	for (auto [entity_id, selected] : level.GetEntitiesWith<SelectedComponentVariant>())
	{
		level.GetComponent<FillColor>().erase(entity_id);
	}
	level.GetComponent<SelectedComponentVariant>().clear();
}

class GridPositionSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		GridPositionManager* grid_position_manager = GetSingleton<GridPositionManager>(level);

		auto level_grid_size = level.GetGridSize();
		auto level_size = level.GetSize();
		sf::Vector2i last_frame_cursor_grid_position = WorldPosToGridPos(cursor_and_keys_.last_frame_cursor_position, BLOCK_SIZE, level_grid_size);
		sf::Vector2i cursor_grid_position = WorldPosToGridPos(cursor_and_keys_.cursor_position, BLOCK_SIZE, level_grid_size);

		for (auto const& [entity_id, button_bound_edit_tool] : level.GetEntitiesWith<ButtonBoundEditTool>())
		{
			sf::Vector2i mouse_button_last_pressed_grid_position = WorldPosToGridPos(cursor_and_keys_.mouse_button_last_pressed_position[button_bound_edit_tool->button], BLOCK_SIZE, level_grid_size);
			if (0 <= cursor_and_keys_.cursor_position.x && cursor_and_keys_.cursor_position.x < level_size.x
				&& 0 <= cursor_and_keys_.cursor_position.y && cursor_and_keys_.cursor_position.y < level_size.y
				&& (cursor_and_keys_.mouse_button_pressed_this_frame[button_bound_edit_tool->button]
					|| (cursor_and_keys_.mouse_button_down[button_bound_edit_tool->button]
						&& last_frame_cursor_grid_position != cursor_grid_position)))
			{
				level.DeleteEntitiesWith<EditModeTemporary>();
				DeselectAll<TemporarilySelected>(level);
				int i_incr = Sign(cursor_grid_position.x - mouse_button_last_pressed_grid_position.x);
				for (int i = mouse_button_last_pressed_grid_position.x; i != cursor_grid_position.x + i_incr; i += i_incr)
				{
					int j_incr = Sign(cursor_grid_position.y - mouse_button_last_pressed_grid_position.y);
					for (int j = mouse_button_last_pressed_grid_position.y; j != cursor_grid_position.y + j_incr; j += j_incr)
					{
						std::string tool = button_bound_edit_tool->tool;
						if (tool == "_select")
						{
							if (grid_position_manager->entity_id_at_position.count({ i, j }))
							{
								TemporarilySelectEntity(level, grid_position_manager->entity_id_at_position.at({ i, j }));
							}
						}
						else
						{
							auto [entity_id, grid_position] = level.AddBlueprintAddComponents<GridPosition>(tool);
							grid_position->grid_position = sf::Vector2i(i, j);
							level.AddComponents<EditModeTemporary>(entity_id);
							grid_position_manager->dirty = true;
						}
					}
				}
			}
			if (cursor_and_keys_.mouse_button_released_this_frame[button_bound_edit_tool->button])
			{
				for (auto [entity_id, temporarity_selected] : level.GetEntitiesWith<TemporarilySelected>())
				{
					SelectEntity(level, entity_id);
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
		}

		// Delete entities:
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DELETE_ENTITY])
		{
			level.DeleteEntitiesWith<Selected>();
			level.DeleteEntitiesWith<TemporarilySelected>();
		}

		if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left] && level.GetEntitiesWith<PressedThisFrame, Selected>().size() == 0
			&& !cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY] && !cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES])
		{
			DeselectAll<Selected>(level);
		}

		/*
		if (level.GetMode() == EDIT_MODE && cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Button::Right])
		{
			auto cursor_grid_pos = WorldPosToGridPos(cursor_and_keys_.cursor_position, BLOCK_SIZE);
			auto grid_index = Vector2iToArray(cursor_grid_pos);
			level.DeleteEntity(grid_position_manager->entity_id_at_position[grid_index]);
			grid_position_manager->dirty = true;
		}*/

		for (auto [entity_id, delete_indicator] : level.GetEntitiesWith<DeleteIndicator>())
		{
			if (!level.HasComponents<EditModeTemporary>(entity_id))
			{
				level.DeleteEntity(entity_id);
			}
		}

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