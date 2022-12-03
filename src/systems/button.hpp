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
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, ClickedOn>& clicked_on_map = level.GetComponent<ClickedOn>();
		std::map<int, Button>& button_map = level.GetComponent<Button>();

		for (auto const& [entity_id, button] : button_map)
		{
			if (clicked_on_map[entity_id].clicked_this_frame)
			{
				button_map[entity_id].on_click();
			}
		}
	}
};