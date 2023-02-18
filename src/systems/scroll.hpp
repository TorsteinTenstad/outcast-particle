#pragma once

#include "game_system.hpp"
#include "level.hpp"

class ScrollSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, scroll_window] : level.GetEntitiesWith<ScrollWindow>())
		{
			for (auto position : scroll_window->positions)
			{
				position->position.y += cursor_and_keys_.mouse_wheel_delta * 180;
			}
		}
	}
};