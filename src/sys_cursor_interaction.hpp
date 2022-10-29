#pragma once
#include "comp_clicked_on.hpp"
#include "comp_physics.hpp"
#include "comp_radius.hpp"
#include "cursor_and_keys.hpp"
#include "utilityfunctions.hpp"
#include <map>

#include <iostream>

class CursorInteractionSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, std::map<int, Draggable>& draggable_map, std::map<int, Radius>& radius_map_, std::map<int, ClickedOn>& clicked_on_map, std::map<int, Position>& position_map_)
	{
		(void)clicked_on_map;
		for (auto& [entity_id, draggable_entity] : draggable_map)
		{
			if (draggable_entity.being_dragged)
			{
				if (cursor_and_keys.left_button_is_pressed)
				{
					position_map_[entity_id].position = cursor_and_keys.cursor_position - draggable_entity.offset;
				}
				else
				{
					draggable_entity.being_dragged = false;
				}
			}
			else if (cursor_and_keys.left_button_is_pressed && Magnitude(cursor_and_keys.cursor_position - position_map_[entity_id].position) < radius_map_[entity_id].radius)
			{
				draggable_entity.being_dragged = true;
				draggable_entity.offset = cursor_and_keys.cursor_position - position_map_[entity_id].position;
			}
		}
	}
};