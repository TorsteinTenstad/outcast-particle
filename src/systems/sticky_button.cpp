#include "_pure_DO_systems.hpp"
#include "level.hpp"
#include <cassert>

void StickyButtonSystem::Update(Level& level, float dt)
{
	assert(level.GetEntitiesWith<StickyButtonDown>().size() <= 1);
	for (auto [entity_id, sticky_button, pressed_this_frame] : level.GetEntitiesWith<StickyButton, PressedThisFrame>())
	{
		bool button_is_down = level.HasComponents<StickyButtonDown>(entity_id);
		for (auto [entity_id, sticky_button, sticky_button_down] : level.GetEntitiesWith<StickyButton, StickyButtonDown>())
		{
			level.RemoveComponents<StickyButtonDown>(entity_id);
		}
		if (!button_is_down)
		{
			level.AddComponent<StickyButtonDown>(entity_id);
		}
	}
}