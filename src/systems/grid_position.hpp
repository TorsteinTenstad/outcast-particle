#pragma once

#include "game_system.hpp"
#include "level.hpp"

class GridPositionSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		GridPositionManager* grid_position_manager = GetSingleton<GridPositionManager>(level);
		if (!grid_position_manager->dirty)
		{
			return;
		}
		grid_position_manager->dirty = false;
		for (auto [entity_id, grid_position, position] : level.GetEntitiesWith<GridPosition, Position>())
		{
			position->position = sf::Vector2f(grid_position->grid_position) * float(BLOCK_SIZE);
			assert(grid_position_manager->entity_id_at_position.count(grid_position->grid_position) == 0);
			grid_position_manager->entity_id_at_position[grid_position->grid_position] = entity_id;
		}
	}
};