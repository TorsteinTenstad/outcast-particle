#pragma once

#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class AnimatedPositionSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, animated_position, position] : level.GetEntitiesWith<AnimatedPosition, Position>())
		{
			position->position = animated_position->position_at_time_func(globals.time - animated_position->start_time);
		}
	}
	
};