#include "components/button.hpp"
#include "Components/sound_info.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/scheduled_delete.hpp"
#include "components/size.hpp"
#include "components/sticky_button.hpp"
#include "components/text.hpp"
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

void ButtonSystem::Update(Level& level, float dt)
{
	for (auto [entity, mouse_interaction_dependent_fill_color, fill_color] : level.GetEntitiesWith<MouseInteractionDependentFillColor, FillColor>())
	{
		if (level.HasComponents<StickyButtonDown>(entity))
		{
			fill_color->color = mouse_interaction_dependent_fill_color->pressed_color;
		}
		else if (level.HasComponents<Pressed>(entity))
		{
			fill_color->color = mouse_interaction_dependent_fill_color->pressed_color;
		}
		else if (level.HasComponents<Hovered>(entity))
		{
			fill_color->color = mouse_interaction_dependent_fill_color->hovered_color;
		}
		else
		{
			fill_color->color = mouse_interaction_dependent_fill_color->default_color;
		}
	}
	for (auto [entity_id, pressed_this_frame, sound_info] : level.GetEntitiesWith<PressedThisFrame, SoundInfo>())
	{
		if (sound_info->sound_paths.count(ON_CLICK) == 1)
		{
			sound_info->play_sound.push(ON_CLICK);
		}
	}

	for (auto [entity, key_config_button, sticky_button_down] : level.GetEntitiesWith<KeyConfigButton, StickyButtonDown>())
	{
		for (const auto& [key, pressed_this_frame] : cursor_and_keys_.key_pressed_this_frame)
		{
			if (key == (sf::Keyboard::Escape) && pressed_this_frame)
			{
				level.RemoveComponents<StickyButtonDown>(entity);
				CreateTextPopup(level, sf::Vector2f(4.5, 1) * float(BLOCK_SIZE), "Escape can't be \n bound as key", 3, false);
				continue;
			}
			if (pressed_this_frame)
			{
				*key_config_button->key = (sf::Keyboard::Key)key;
				level.RemoveComponents<StickyButtonDown>(entity);
				*(key_config_button->button_text) = HumanName(*key_config_button->key);
			}
		}
	}

	for (auto [entity, released_this_frame, on_released_this_frame, binary_options_button] : level.GetEntitiesWith<ReleasedThisFrame, OnReleasedThisFrame, BinaryOptionsButton>())
	{
		*(binary_options_button->button_text) = BoolToStringAsEnabledOrDisabled(*(binary_options_button->button_text) == "Disabled");
	}
	for (auto [entity, pressed, slider_button] : level.GetEntitiesWith<Pressed, SliderButton>())
	{
		float slider_x_pos = level.GetComponent<Position>(slider_button->slider_bar)->position.x;
		float relative_cursor_x_pos = cursor_and_keys_.cursor_position.x - slider_x_pos;
		float half_slider_width = level.GetComponent<WidthAndHeight>(slider_button->slider_bar)->width_and_height.x * 0.5;

		*(slider_button->slider_value) = Clamp(int(50 * relative_cursor_x_pos / half_slider_width + 50), 0, 100);
		level.GetComponent<Position>(slider_button->slider_button)->position.x = std::max(slider_x_pos - half_slider_width, std::min(slider_x_pos + half_slider_width, slider_x_pos + relative_cursor_x_pos));
		level.GetComponent<Text>(slider_button->slider_text)->content = RightShiftString(ToString(*(slider_button->slider_value)), 3);
	}

	for (auto [entity, hovered_started_this_frame, on_hovered_started_this_frame] : level.GetEntitiesWith<HoveredStartedThisFrame, OnHoveredStartedThisFrame>())
	{
		on_hovered_started_this_frame->func();
		return;
	}
	for (auto [entity, hovered, on_hovered] : level.GetEntitiesWith<Hovered, OnHovered>())
	{
		on_hovered->func();
		return;
	}
	for (auto [entity, released_this_frame, on_released_this_frame] : level.GetEntitiesWith<ReleasedThisFrame, OnReleasedThisFrame>())
	{
		on_released_this_frame->func();
		return;
	}
}
