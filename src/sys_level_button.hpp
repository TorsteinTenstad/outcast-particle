#pragma once
#include "PCH.hpp"
#include "comp_level_button.hpp"
#include "comp_mouse_interactions.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class LevelButtonSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, ClickedOn>& clicked_on_map = level.clicked_on_;
		std::map<int, LevelButton>& level_button_map = level.level_button_;

		for (auto const& [entity_id, level_button] : level_button_map)
		{
			if (clicked_on_map[entity_id].clicked_this_frame)
			{
				globals.active_level = level_button.level;
			}
		}
	}
};