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
		std::map<int, WidthAndHeight>& width_and_height_map = level.GetComponent<WidthAndHeight>();

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
			if (width_and_height_map.count(entity_id))
			{
				float w = width_and_height_map[entity_id].width_and_height.x;
				float h = width_and_height_map[entity_id].width_and_height.y;
				sf::Vector2f offset = Abs(position_map[entity_id].position - cursor_and_keys.cursor_position);
				if (offset.x < w / 2 && offset.y < h / 2)
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