#include "_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"

#include <algorithm>
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
			if (key == (sf::Keyboard::Escape) && pressed_this_frame)
			{
				level.RemoveComponents<StickyButtonDown>(entity_id);
				auto [error_text_id, text, position, draw_priority, scheduled_delete] = level.CreateEntitiyWith<Text, Position, DrawPriority, ScheduledDelete>();
				text->content = "Escape can't be \n bound as key";
				position->position = sf::Vector2f(4.5, 1) * float(BLOCK_SIZE);
				draw_priority->draw_priority = 101;
				scheduled_delete->delete_at = globals.time + 3;
				continue;
			}
			if (pressed_this_frame)
			{
				*key_config_button->key = (sf::Keyboard::Key)key;
				level.RemoveComponents<StickyButtonDown>(entity_id);
				*(key_config_button->button_text) = HumanName(*key_config_button->key);
			}
		}
	}

	for (auto [entity_id, released_this_frame, on_released_this_frame, binary_options_button] : level.GetEntitiesWith<ReleasedThisFrame, OnReleasedThisFrame, BinaryOptionsButton>())
	{
		*(binary_options_button->button_text) = BoolToStringAsEnabledOrDisabled(*(binary_options_button->button_text) == "Disabled");
	}
	for (auto [entity_id, pressed, slider_button] : level.GetEntitiesWith<Pressed, SliderButton>())
	{
		float slider_x_pos = level.GetComponent<Position>(slider_button->slider_bar_id)->position.x;
		float relative_cursor_x_pos = cursor_and_keys_.cursor_position.x - slider_x_pos;
		float half_slider_width = level.GetComponent<WidthAndHeight>(slider_button->slider_bar_id)->width_and_height.x * 0.5;

		*(slider_button->slider_value) = Clamp(int(50 * relative_cursor_x_pos / half_slider_width + 50), 0, 100);
		level.GetComponent<Position>(slider_button->slider_button_id)->position.x = std::max(slider_x_pos - half_slider_width, std::min(slider_x_pos + half_slider_width, slider_x_pos + relative_cursor_x_pos));
		level.GetComponent<Text>(slider_button->slider_text_id)->content = RightShiftString(ToString(*(slider_button->slider_value)), 3);
	}
}
