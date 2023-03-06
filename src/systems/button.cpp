#include "_pure_DO_systems.hpp"
#include "string_parsing_utils.hpp"
#include "utils.hpp"
#include <cmath>
#include <iostream>

void ButtonSystem::Update(Level& level, float dt)
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
		//return;
	}
	for (auto [entity_id, hovered, on_hovered] : level.GetEntitiesWith<Hovered, OnHovered>())
	{
		on_hovered->func();
		//return;
	}
	for (auto [entity_id, released_this_frame, on_released_this_frame] : level.GetEntitiesWith<ReleasedThisFrame, OnReleasedThisFrame>())
	{
		on_released_this_frame->func();
		//return;
	}

	for (auto [entity_id, key_config_button, sticky_button_down] : level.GetEntitiesWith<KeyConfigButton, StickyButtonDown>())
	{
		for (const auto& [key, pressed_this_frame] : cursor_and_keys_.key_pressed_this_frame)
		{
			if (pressed_this_frame)
			{
				*key_config_button->key = (sf::Keyboard::Key)key;
				level.RemoveComponents<StickyButtonDown>(entity_id);
				*(level.AddComponent<KeyConfigButton>(entity_id)->button_text) = HumanName(*key_config_button->key);
			}
		}
	}

	for (auto [entity_id, released_this_frame, on_released_this_frame, binary_options_button] : level.GetEntitiesWith<ReleasedThisFrame, OnReleasedThisFrame, BinaryOptionsButton>())
	{
		*(level.AddComponent<BinaryOptionsButton>(entity_id)->button_text) = BoolToStringAsEnabledOrDisabled(*(level.GetComponent<BinaryOptionsButton>(entity_id)->button_text) == "Disabled");
		//return;
	}
	for (auto [entity_id, position, pressed, slider_button, width_and_height] : level.GetEntitiesWith<Position, Pressed, SliderButton, WidthAndHeight>())
	{
		float relative_cursor_x_pos = cursor_and_keys_.cursor_position.x - position->position.x;
		float half_button_width = width_and_height->width_and_height.x / 2;
		if (abs(relative_cursor_x_pos) > half_button_width)
		{
			continue;
		}
		float intermediary_slider_value = 50 * relative_cursor_x_pos / half_button_width + 50;
		*(slider_button->slider_value) = intermediary_slider_value;
		std::cout << *(slider_button->slider_value) << std::endl;
	}
}
