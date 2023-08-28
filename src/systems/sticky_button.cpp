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
		if (!sticky_button->channel)
		{
			button_is_down ? level.RemoveComponents<StickyButtonDown>(entity) : level.AddComponent<StickyButtonDown>(entity);
			return;
		}

		int channel = sticky_button->channel.value();
		for (auto [entity, sticky_button, sticky_button_down] : level.GetEntitiesWith<StickyButton, StickyButtonDown>())
		{
			if (sticky_button->channel == channel) { level.RemoveComponents<StickyButtonDown>(entity); }
		}
		if (sticky_button->enforce_down || !button_is_down)
		{
			level.EnsureExistenceOfComponent<StickyButtonDown>(entity);
		}
	}
}