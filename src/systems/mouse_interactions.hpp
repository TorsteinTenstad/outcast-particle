#pragma once
#include "components/area.hpp"
#include "components/physics.hpp"
#include "components/pressed.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <algorithm>

class MouseInterationSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		level.GetComponent<ReleasedThisFrame>().clear();
		level.GetComponent<PressedThisFrame>().clear();
		if (cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Left])
		{
			for (auto& [entity_id, pressed] : level.GetEntitiesWith<Pressed>())
			{
				level.GetComponent<ReleasedThisFrame>()[entity_id];
			}
			level.GetComponent<Pressed>().clear();
		}

		if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left])
		{
			std::vector<std::tuple<int, int>> entities_intersecting_mouse; // contains (draw_priority, entity_id)

			for (auto [entity_id, radius, can_receive_press, draw_priority, position] : level.GetEntitiesWith<Radius, CanReceivePress, DrawPriority, Position>())
			{
				if (Magnitude(cursor_and_keys_.cursor_position - position->position) < radius->radius)
				{
					entities_intersecting_mouse.push_back({ draw_priority->draw_priority, entity_id });
				}
			}
			for (auto [entity_id, width_and_height, can_receive_press, draw_priority, position] : level.GetEntitiesWith<WidthAndHeight, CanReceivePress, DrawPriority, Position>())
			{
				float w = width_and_height->width_and_height.x;
				float h = width_and_height->width_and_height.y;
				sf::Vector2f offset = Abs(position->position - cursor_and_keys_.cursor_position);
				if (offset.x < w / 2 && offset.y < h / 2)
				{
					entities_intersecting_mouse.push_back({ draw_priority->draw_priority, entity_id });
				}
			}
			if (entities_intersecting_mouse.size() > 0)
			{
				int top_intersecting_id = std::get<1>(*max_element(entities_intersecting_mouse.begin(), entities_intersecting_mouse.end()));
				level.GetComponent<PressedThisFrame>()[top_intersecting_id];
				level.GetComponent<Pressed>()[top_intersecting_id];
			}
		}
	}
	
};