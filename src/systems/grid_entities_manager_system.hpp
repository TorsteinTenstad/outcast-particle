#pragma once
#include "SFML/Window/Mouse.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include <cmath>

#define MAX_COLS 32
#define MAX_ROWS 18

static sf::Vector2i WorldPosToGridPos(sf::Vector2f world_pos, float grid_size)
{
	sf::Vector2f unrounded_grid_pos = world_pos / grid_size;
	return sf::Vector2i((int)std::floor(unrounded_grid_pos.x), (int)std::floor(unrounded_grid_pos.y));
}

class GridEntitiesManagerSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		if (level.GetEntitiesWith<GridEntitiesManager>().size() == 0)
		{
			int id = level.AddBlueprint("BPGridEntitiesManager");
			Shader* shader = &level.GetComponent<Shader>()[id];
			for (int i = 0; i < MAX_COLS * MAX_ROWS; i++)
			{
				shader->int_uniforms["grid_entity[" + ToString(i) + "]"] = EMPTY;
			}
		}
		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::G])
		{
			auto [prev_id, prev_grid, prev_shader] = level.GetEntitiesWith<GridEntitiesManager, Shader>()[0];
			GridEntitiesManager grid = *prev_grid;
			Shader shader = *prev_shader;
			level.DeleteEntitiesWith<GridEntitiesManager>();
			int id = level.AddBlueprint("BPGridEntitiesManager");
			level.GetComponent<GridEntitiesManager>()[id] = grid;
			level.GetComponent<Shader>()[id] = shader;
		}
		for (auto [entity_id, grid_entities_manager, shader, width_and_height, position] : level.GetEntitiesWith<GridEntitiesManager, Shader, WidthAndHeight, Position>())
		{
			sf::Vector2f level_size = level.GetSize();
			width_and_height->width_and_height = level_size;
			position->position = level_size / 2.f;

			sf::Vector2i mouse_grid_pos = WorldPosToGridPos(cursor_and_keys_.cursor_position, grid_entities_manager->grid_size);
			if (cursor_and_keys_.mouse_button_down[sf::Mouse::Button::Left])
			{
				grid_entities_manager->grid[mouse_grid_pos.x][mouse_grid_pos.y] = WALL;
				shader->int_uniforms["grid_entity[" + ToString(mouse_grid_pos.y * MAX_COLS + mouse_grid_pos.x) + "]"] = WALL;
			}
			if (cursor_and_keys_.mouse_button_down[sf::Mouse::Button::Right])
			{
				grid_entities_manager->grid[mouse_grid_pos.x][mouse_grid_pos.y] = EMPTY;
				shader->int_uniforms["grid_entity[" + ToString(mouse_grid_pos.y * MAX_COLS + mouse_grid_pos.x) + "]"] = EMPTY;
			}
		}
	}
};