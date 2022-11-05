#pragma once
#include "components/area.hpp"
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

class MouseInterationSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)dt;
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, ClickedOn>& clicked_on_map = level.GetComponent<ClickedOn>();
		std::map<int, Radius>& radius_map = level.GetComponent<Radius>();
		std::map<int, WidthAndHight>& width_and_hight_map = level.GetComponent<WidthAndHight>();

		for (auto& [entity_id, clicked_on] : clicked_on_map)
		{
			clicked_on.clicked_this_frame = false;
			clicked_on.released_this_frame = cursor_and_keys.mouse_button_released_this_frame[sf::Mouse::Left];
			if (radius_map.count(entity_id))
			{
				if (Magnitude(cursor_and_keys.cursor_position - position_map[entity_id].position) < radius_map[entity_id].radius)
				{
					clicked_on.clicked_this_frame = cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left];
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
					clicked_on.clicked_this_frame = cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left];
				}
			}
			if (clicked_on.clicked_this_frame)
			{
				clicked_on.clicked_on = true;
			}
			if (clicked_on.released_this_frame)
			{
				clicked_on.clicked_on = false;
			}
		}
	}
};