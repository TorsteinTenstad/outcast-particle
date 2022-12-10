#pragma once
#include "components/area.hpp"
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

class MouseInterationSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& position_map = level.GetComponent<Position>();
		auto& clicked_on_map = level.GetComponent<ClickedOn>();
		auto& radius_map = level.GetComponent<Radius>();
		auto& width_and_height_map = level.GetComponent<WidthAndHeight>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& draw_priority_map = level.GetComponent<DrawPriority>();

		std::map<int, std::vector<int>> priority;
		for (auto& [entity_id, clicked_on] : clicked_on_map)
		{
			priority[draw_priority_map[entity_id].draw_priority].push_back(entity_id);
		}
		for (auto it = priority.rbegin(); it != priority.rend(); it++)
		{
			for (auto entity_id : it->second)
			{
				clicked_on_map[entity_id].clicked_this_frame = false;
				clicked_on_map[entity_id].released_this_frame = clicked_on_map[entity_id].clicked_on && cursor_and_keys.mouse_button_released_this_frame[sf::Mouse::Left];
				if (radius_map.count(entity_id))
				{
					if (Magnitude(cursor_and_keys.cursor_position - position_map[entity_id].position) < radius_map[entity_id].radius)
					{
						clicked_on_map[entity_id].clicked_this_frame = cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left];
					}
				}
				if (width_and_height_map.count(entity_id))
				{
					float w = width_and_height_map[entity_id].width_and_height.x;
					float h = width_and_height_map[entity_id].width_and_height.y;
					sf::Vector2f offset = Abs(position_map[entity_id].position - cursor_and_keys.cursor_position);
					if (offset.x < w / 2 && offset.y < h / 2)
					{
						clicked_on_map[entity_id].clicked_this_frame = cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left];
					}
				}
				if (clicked_on_map[entity_id].clicked_this_frame)
				{
					clicked_on_map[entity_id].clicked_on = true;
					return;
				}
				if (clicked_on_map[entity_id].released_this_frame)
				{
					clicked_on_map[entity_id].clicked_on = false;
					return;
				}
			}
		}
	}
};