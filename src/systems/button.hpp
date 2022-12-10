#pragma once
#include "PCH.hpp"
#include "components/button.hpp"
#include "components/mouse_interactions.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class ButtonSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& clicked_on_map = level.GetComponent<ClickedOn>();
		auto& button_map = level.GetComponent<Button>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();

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
	}
};