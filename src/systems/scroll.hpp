#pragma once

#include "game_system.hpp"
#include "level.hpp"

class ScrollSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, scroll_window, width_and_height, position] : level.GetEntitiesWith<ScrollWindow, WidthAndHeight, Position>())
		{
			for (auto position : scroll_window->positions)
			{
				position->position.y += cursor_and_keys_.mouse_wheel_delta * 180;
			}
			for (auto shader : scroll_window->shaders)
			{
				shader->float_uniforms["top"] = position->position.y - width_and_height->width_and_height.y / 2;
				shader->float_uniforms["bottom"] = position->position.y + width_and_height->width_and_height.y / 2;
			}
		}
	}
};