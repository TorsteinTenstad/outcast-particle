#pragma once

#include "components/button.hpp"
#include "components/button_events.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"
#include "utils.hpp"

class ButtonSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, mouse_interaction_dependent_fill_color, fill_color] : level.GetEntitiesWith<MouseInteractionDependentFillColor, FillColor>())
		{
			if (level.HasComponents<StickyButtonDown>(entity_id))
			{
				fill_color->color = mouse_interaction_dependent_fill_color->pressed_color;
			}
			else if (level.HasComponents<Pressed>(entity_id))
			{
				fill_color->color = mouse_interaction_dependent_fill_color->pressed_color;
			}
			else if (level.HasComponents<Hovered>(entity_id))
			{
				fill_color->color = mouse_interaction_dependent_fill_color->hovered_color;
			}
			else
			{
				fill_color->color = mouse_interaction_dependent_fill_color->default_color;
			}
		}

		for (auto [entity_id, hovered_started_this_frame, on_hovered_started_this_frame] : level.GetEntitiesWith<HoveredStartedThisFrame, OnHoveredStartedThisFrame>())
		{
			on_hovered_started_this_frame->func();
			return;
		}
		for (auto [entity_id, hovered, on_hovered] : level.GetEntitiesWith<Hovered, OnHovered>())
		{
			on_hovered->func();
			return;
		}
		for (auto [entity_id, released_this_frame, on_released_this_frame] : level.GetEntitiesWith<ReleasedThisFrame, OnReleasedThisFrame>())
		{
			on_released_this_frame->func();
			return;
		}

		for (auto [entity_id, key_config_button, sticky_button_down, entity_link] : level.GetEntitiesWith<KeyConfigButton, StickyButtonDown, EntityLink>())
		{
			for (const auto& [key, pressed_this_frame] : cursor_and_keys_.key_pressed_this_frame)
			{
				if (pressed_this_frame)
				{
					*key_config_button->key = (sf::Keyboard::Key)key;
					level.RemoveComponents<StickyButtonDown>(entity_id);
					level.AddComponent<Text>(entity_link->entity_link)->content = HumanName(*key_config_button->key);
				}
			}
		}
	}
};