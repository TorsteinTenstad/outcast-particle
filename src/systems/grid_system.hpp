#pragma once

#include "SFML/Window/Mouse.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include <cmath>

std::array<int, 2> WorldPosToGridPos(sf::Vector2f world_pos, float grid_size)
{
	sf::Vector2f unrounded_grid_pos = world_pos / grid_size;
	return { (int)std::floor(unrounded_grid_pos.x), (int)std::floor(unrounded_grid_pos.y) };
}

class GridSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, grid] : level.GetEntitiesWith<Grid>())
		{
			std::array<int, 2> mouse_grid_pos = WorldPosToGridPos((sf::Vector2f)sf::Mouse::getPosition(globals.render_window), grid->grid_size);
			if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Button::Left])
			{
				grid->grid[mouse_grid_pos[0]][mouse_grid_pos[1]] = true;
			}
			if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Button::Left])
			{
				grid->grid[mouse_grid_pos[0]][mouse_grid_pos[1]] = false;
			}
		}
	}
};