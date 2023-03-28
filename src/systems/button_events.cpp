#include "_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include <algorithm>

// Return vector has elements containing (draw_priority, entity_id)
std::vector<std::tuple<int, int>> GetEntitiesIntersectingMouse(Level& level, sf::Vector2f cursor_position)
{
	std::vector<std::tuple<int, int>> entities_intersecting_mouse;
	for (auto [entity_id, radius, can_receive_press, draw_priority, position] : level.GetEntitiesWith<Radius, ReceivesButtonEvents, DrawPriority, Position>())
	{
		if (Magnitude(cursor_position - position->position) < radius->radius)
		{
			entities_intersecting_mouse.push_back({ draw_priority->draw_priority, entity_id });
		}
	}
	for (auto [entity_id, width_and_height, can_receive_press, draw_priority, position] : level.GetEntitiesWith<WidthAndHeight, ReceivesButtonEvents, DrawPriority, Position>())
	{
		float w = width_and_height->width_and_height.x;
		float h = width_and_height->width_and_height.y;
		sf::Vector2f offset = Abs(position->position - cursor_position);
		if (offset.x < w / 2 && offset.y < h / 2)
		{
			entities_intersecting_mouse.push_back({ draw_priority->draw_priority, entity_id });
		}
	}
	return entities_intersecting_mouse;
}

void ButtonEventsSystem::Update(Level& level, float dt)
{
	level.ClearComponent<ReleasedThisFrame>();
	level.ClearComponent<PressedThisFrame>();
	level.ClearComponent<HoveredStartedThisFrame>();

	if (cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Left])
	{
		for (auto& [entity_id, pressed] : level.GetEntitiesWith<Pressed>())
		{
			level.AddComponent<ReleasedThisFrame>(entity_id);
		}
		level.ClearComponent<Pressed>();
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
			level.RemoveComponents<Pressed>(entity_id);
		}
	}

	std::vector<std::tuple<int, bool, bool, bool>> hovering_entity_ids = {};

	std::vector<std::tuple<int, int>> entities_intersecting_mouse = {};
	entities_intersecting_mouse = GetEntitiesIntersectingMouse(level, cursor_and_keys_.cursor_position);

	auto entities_with_menu_navigator = level.GetEntitiesWith<MenuNavigator>();

	if (entities_intersecting_mouse.size() > 0)
	{
		int top_intersecting_id = std::get<1>(*max_element(entities_intersecting_mouse.begin(), entities_intersecting_mouse.end()));

		bool mouse_hover_inactive =
			entities_with_menu_navigator.size() > 0
			&& !cursor_and_keys_.cursor_moved_this_frame
			&& !cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left]
			&& !cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Left]
			&& level.HasComponents<MenuNavigable>(top_intersecting_id);

		if (!mouse_hover_inactive)
		{
			hovering_entity_ids.push_back({ top_intersecting_id,
				cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left],
				cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Left],
				level.HasComponents<Hovered>(top_intersecting_id) });
		}
	}

	for (auto [entity_id, menu_navigator] : entities_with_menu_navigator)
	{
		int at_id = menu_navigator->currently_at_entity_id;
		if (level.HasComponents<ReceivesButtonEvents>(at_id))
		{
			hovering_entity_ids.push_back({ at_id,
				cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Enter],
				cursor_and_keys_.key_released_this_frame[sf::Keyboard::Enter],
				level.HasComponents<Hovered>(at_id) });
		}
	}

	level.ClearComponent<Hovered>();

	for (auto [entity_id, pressed_this_frame, released_this_frame, hovered_last_frame] : hovering_entity_ids)
	{
		if (pressed_this_frame)
		{
			level.EnsureExistenceOfComponent<PressedThisFrame>(entity_id);
			level.EnsureExistenceOfComponent<Pressed>(entity_id);
		}
		if (released_this_frame)
		{
			level.EnsureExistenceOfComponent<ReleasedThisFrame>(entity_id);
			level.RemoveComponents<Pressed>(entity_id);
		}

		level.EnsureExistenceOfComponent<Hovered>(entity_id);
		if (!hovered_last_frame)
		{
			level.EnsureExistenceOfComponent<HoveredStartedThisFrame>(entity_id);
		}
	}
}