#pragma once
#include "PCH.hpp"
#include "components/button.hpp"
#include "components/mouse_interactions.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"
#include "utils.hpp"

class ButtonSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& clicked_on_map = level.GetComponent<ClickedOn>();
		auto& button_map = level.GetComponent<Button>();
		auto& key_config_button_map = level.GetComponent<KeyConfigButton>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& text_map = level.GetComponent<Text>();

		for (auto const& [entity_id, button] : button_map)
		{
			if (clicked_on_map[entity_id].clicked_this_frame)
			{
				if (!button.pressed_image_path.empty())
				{
					draw_info_map[entity_id].image_path = button.pressed_image_path;
				}
			}
			else if (clicked_on_map[entity_id].released_this_frame)
			{
				if (!button.image_path.empty())
				{
					draw_info_map[entity_id].image_path = button.image_path;
				}
				button.on_click();
			}
		}
		for (auto& [entity_id, key_config_button] : key_config_button_map)
		{
			if (clicked_on_map[entity_id].clicked_this_frame)
			{
				for (auto& [id, release_button] : key_config_button_map)
				{
					release_button.is_pressed = false;
					if (!release_button.image_path.empty())
					{
						draw_info_map[id].image_path = release_button.image_path;
					}
				}

				if (!key_config_button.pressed_image_path.empty())
				{
					draw_info_map[entity_id].image_path = key_config_button.pressed_image_path;
					key_config_button.is_pressed = true;
				}
				break;
			}
		}
		for (auto& [entity_id, key_config_button] : key_config_button_map)
		{
			if (key_config_button.is_pressed)
			{
				for (const auto& [key, pressed_this_frame] : cursor_and_keys.key_pressed_this_frame)
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