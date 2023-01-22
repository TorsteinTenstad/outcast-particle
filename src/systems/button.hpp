#pragma once

#include "components/button.hpp"
#include "components/pressed.hpp"
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
		auto& draw_info_map = level.GetComponent<DrawInfo>();

		for (auto [entity_id, pressed_this_frame, pressed_image_path] : level.GetEntitiesWith<PressedThisFrame, PressedImagePath>())
		{
			draw_info_map[entity_id].image_path = pressed_image_path->pressed_image_path;
		}
		for (auto [entity_id, released_this_frame, on_released_this_frame, pressed_image_path] : level.GetEntitiesWith<ReleasedThisFrame, OnReleasedThisFrame, PressedImagePath>())
		{
			draw_info_map[entity_id].image_path = pressed_image_path->image_path;
			on_released_this_frame->func();
			return;
		}
		for (auto [entity_id, hovered, on_hovered] : level.GetEntitiesWith<Hovered, OnHovered>())
		{
			on_hovered->func();
			return;
		}
		for (auto [entity_id, hovered_started_this_frame, on_hovered_started_this_frame] : level.GetEntitiesWith<HoveredStartedThisFrame, OnHoveredStartedThisFrame>())
		{
			on_hovered_started_this_frame->func();
			return;
		}

		auto& key_config_button_map = level.GetComponent<KeyConfigButton>();
		auto& text_map = level.GetComponent<Text>();
		for (auto [entity_id, pressed_this_frame, key_config_button] : level.GetEntitiesWith<PressedThisFrame, KeyConfigButton>())
		{
			for (auto& [id, release_button] : key_config_button_map)
			{
				release_button.is_pressed = false;
				if (!release_button.image_path.empty())
				{
					draw_info_map[id].image_path = release_button.image_path;
				}
			}

			if (!key_config_button->pressed_image_path.empty())
			{
				draw_info_map[entity_id].image_path = key_config_button->pressed_image_path;
				key_config_button->is_pressed = true;
			}
			break;
		}

		for (auto& [entity_id, key_config_button] : key_config_button_map)
		{
			if (key_config_button.is_pressed)
			{
				for (const auto& [key, pressed_this_frame] : cursor_and_keys_.key_pressed_this_frame)
				{
					if (pressed_this_frame)
					{
						*key_config_button.key = (sf::Keyboard::Key)key;
						key_config_button.is_pressed = false;
						if (!key_config_button.image_path.empty())
						{
							draw_info_map[entity_id].image_path = key_config_button.image_path;
						}

						// Set button text:
						assert(text_map.count(entity_id) > 0);
						std::vector<std::string> button_description = SplitString(text_map[entity_id].content, " ");
						text_map[entity_id].content = "";
						for (unsigned i = 0; i < button_description.size() - 1; ++i)
						{
							text_map[entity_id].content += button_description[i] + " ";
						}
						text_map[entity_id].content += HumanName((sf::Keyboard::Key)key);
					}
				}
			}
		}
	}
};