#pragma once
#include "comp_clicked_on.hpp"
#include "comp_cursor.hpp"
#include "comp_physics.hpp"
#include "comp_radius.hpp"
#include "utilityfunctions.hpp"
#include <map>

#include <iostream>

class CursorInteractionSystem
{
public:
	void Update(Cursor& cursor, std::map<int, Draggable>& draggable_map, std::map<int, Radius>& radius_map_, std::map<int, ClickedOn>& clicked_on_map, std::map<int, Position>& position_map_)
	{
		(void)clicked_on_map;
		for (auto& [entity_id, draggable_entity] : draggable_map)
		{
			if (draggable_entity.being_dragged)
			{
				if (cursor.left_button_is_pressed)
				{
					position_map_[entity_id].position = cursor.position;
				}
				else
				{
					draggable_entity.being_dragged = false;
				}
			}
			else if (cursor.left_button_is_pressed && Magnitude(cursor.position - position_map_[entity_id].position) < radius_map_[entity_id].radius)
			{
				draggable_entity.being_dragged = true;
			}
		}
	}
};