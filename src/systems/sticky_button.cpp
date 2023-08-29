#include "components/sticky_button.hpp"
#include "components/button_events.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include <cassert>

void StickyButtonSystem::Update(Level& level, float dt)
{
	for (auto [entity, sticky_button, pressed_this_frame] : level.GetEntitiesWith<StickyButton, PressedThisFrame>())
	{
		bool button_is_down = level.HasComponents<StickyButtonDown>(entity);
		level.RemoveComponent<StickyButtonDown>(entity);

		for (auto [other_entity, other_sticky_button, other_sticky_button_down] : level.GetEntitiesWith<StickyButton, StickyButtonDown>())
		{
			if (sticky_button->channel.has_value() && sticky_button->channel == other_sticky_button->channel) { level.RemoveComponents<StickyButtonDown>(other_entity); }
		}
		if (sticky_button->enforce_down || !button_is_down)
		{
			level.EnsureExistenceOfComponent<StickyButtonDown>(entity);
		}
	}
}