#pragma once

#include "game_system.hpp"
#include "level.hpp"

class StickyButtonSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, sticky_button, pressed_this_frame] : level.GetEntitiesWith<StickyButton, PressedThisFrame>())
		{
			for (auto [entity_id, sticky_button, sticky_button_down] : level.GetEntitiesWith<StickyButton, StickyButtonDown>())
			{
				level.RemoveComponents<StickyButtonDown>(entity_id);
			}
			level.AddComponent<StickyButtonDown>(entity_id);
		}
	}
};