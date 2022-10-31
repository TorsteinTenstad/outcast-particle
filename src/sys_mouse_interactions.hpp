#pragma once
#include "comp_area.hpp"
#include "comp_mouse_interactions.hpp"
#include "comp_physics.hpp"
#include "cursor_and_keys.hpp"
#include "utilityfunctions.hpp"

class MouseInterationSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, std::map<int, ClickedOn>& clicked_on_map, std::map<int, Position>& position_map, std::map<int, Radius>& radius_map, std::map<int, WidthAndHight>& width_and_hight_map)
	{
		for (auto& [entity_id, clicked_on] : clicked_on_map)
		{
			if (radius_map.count(entity_id))
			{
				if (Magnitude(cursor_and_keys.cursor_position - position_map[entity_id].position) < radius_map[entity_id].radius)
				{
					clicked_on.clicked_on = cursor_and_keys.mouse_button_down[sf::Mouse::Left];
					clicked_on.clicked_this_frame = cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left];
					clicked_on.released_this_frame = cursor_and_keys.mouse_button_released_this_frame[sf::Mouse::Left];
				}
			}
			if (width_and_hight_map.count(entity_id))
			{
				float w = width_and_hight_map[entity_id].width_and_hight.x;
				float h = width_and_hight_map[entity_id].width_and_hight.y;
				float e_x = position_map[entity_id].position.x;
				float e_y = position_map[entity_id].position.y;
				float m_x = cursor_and_keys.cursor_position.x;
				float m_y = cursor_and_keys.cursor_position.y;
				if (e_x < m_x && m_x < e_x + w && e_y < m_y && m_y < e_y + h)
				{
					clicked_on.clicked_on = cursor_and_keys.mouse_button_down[sf::Mouse::Left];
					clicked_on.clicked_this_frame = cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left];
					clicked_on.released_this_frame = cursor_and_keys.mouse_button_released_this_frame[sf::Mouse::Left];
				}
			}
		}
	}
};