#pragma once
#include "PCH.hpp"
#include "comp_level_button.hpp"
#include "comp_mouse_interactions.hpp"
#include "globals.hpp"

class LevelButtonSystem
{
public:
	void Update(std::map<int, ClickedOn>& clicked_on_map, std::map<int, LevelButton>& level_button_map)
	{
		for (auto const& [entity_id, level_button] : level_button_map)
		{
			std::cout << clicked_on_map[entity_id].clicked_this_frame << std::endl;
			if (clicked_on_map[entity_id].clicked_this_frame)
			{
				globals.active_level = level_button.level;
			}
		}
	}
};