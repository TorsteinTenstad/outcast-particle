#pragma once
#include "components/area.hpp"
#include "components/input_events.hpp"
#include "components/physics.hpp"
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
		level.GetComponent<HoveredStartedThisFrame>().clear();
		if (cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Left])
		{
			for (auto& [entity_id, pressed] : level.GetEntitiesWith<Pressed>())
			{
				level.GetComponent<ReleasedThisFrame>()[entity_id];
			}
			level.GetComponent<Pressed>().clear();
		}

		for (auto [entity_id, shortcut_key] : level.GetEntitiesWith<ShortcutKey>())
		{
			if (cursor_and_keys_.key_pressed_this_frame[shortcut_key->key])
			{
				level.AddComponent<PressedThisFrame>(entity_id);
				level.AddComponent<Pressed>(entity_id);
			}
		}

		for (auto [entity_id, shortcut_key, pressed] : level.GetEntitiesWith<ShortcutKey, Pressed>())
		{
			if (cursor_and_keys_.key_released_this_frame[shortcut_key->key])
			{
				level.AddComponent<ReleasedThisFrame>(entity_id);
			}
		}

		std::vector<std::tuple<int, int>> entities_intersecting_mouse; // contains (draw_priority, entity_id)

		for (auto [entity_id, radius, can_receive_press, draw_priority, position] : level.GetEntitiesWith<Radius, ReceivesMouseEvents, DrawPriority, Position>())
		{
			if (Magnitude(cursor_and_keys_.cursor_position - position->position) < radius->radius)
			{
				entities_intersecting_mouse.push_back({ draw_priority->draw_priority, entity_id });
			}
		}
		for (auto [entity_id, width_and_height, can_receive_press, draw_priority, position] : level.GetEntitiesWith<WidthAndHeight, ReceivesMouseEvents, DrawPriority, Position>())
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

			if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left])
			{
				level.GetComponent<PressedThisFrame>()[top_intersecting_id];
				level.GetComponent<Pressed>()[top_intersecting_id];
			}
			else
			{
				bool hovered_last_frame = level.HasComponents<Hovered>(top_intersecting_id);
				if (!hovered_last_frame)
				{
					level.AddComponents<HoveredStartedThisFrame>(top_intersecting_id);
					level.GetComponent<Hovered>().clear();
					level.AddComponents<Hovered>(top_intersecting_id);
				}
			}
		}
	}
};