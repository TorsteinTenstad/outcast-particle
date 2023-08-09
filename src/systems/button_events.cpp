#include "components/button_events.hpp"
#include "Components/sound_info.hpp"
#include "components/button.hpp"
#include "components/draw_info.hpp"
#include "components/menu_navigator.hpp"
#include "components/position.hpp"
#include "components/scroll.hpp"
#include "components/size.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include <algorithm>

bool PointIsInside(sf::Vector2f point, WidthAndHeight* width_and_height, Position* position)
{
	sf::Vector2f offset = Abs(position->position - point);
	return (offset.x < width_and_height->width_and_height.x / 2) && (offset.y < width_and_height->width_and_height.y / 2);
}

bool CursorIsOutsideScrollWindowContainingEntity(Level& level, Entity entity, sf::Vector2f cursor_position)
{
	auto scroll_windows = level.GetEntitiesWith<ScrollWindow, WidthAndHeight, Position>();
	for (const auto& [_, scroll_window, width_and_heigh, position] : scroll_windows)
	{
		if (PointIsInside(cursor_position, width_and_heigh, position)) { continue; }
		if (std::find(scroll_window->entities.begin(), scroll_window->entities.end(), entity) == scroll_window->entities.end()) { continue; }
		return true;
	}
	return false;
}

// Return vector has elements containing (draw_priority, entity)
std::vector<std::tuple<Entity, Entity>> GetEntitiesIntersectingMouse(Level& level, sf::Vector2f cursor_position)
{
	std::vector<std::tuple<Entity, Entity>> entities_intersecting_mouse;
	for (auto [entity, radius, can_receive_press, draw_priority, position] : level.GetEntitiesWith<Radius, ReceivesButtonEvents, DrawPriority, Position>())
	{
		if (Magnitude(cursor_position - position->position) > radius->radius) { continue; }
		if (CursorIsOutsideScrollWindowContainingEntity(level, entity, cursor_position)) { continue; }
		entities_intersecting_mouse.push_back({ draw_priority->draw_priority, entity });
	}
	for (auto [entity, width_and_height, can_receive_press, draw_priority, position] : level.GetEntitiesWith<WidthAndHeight, ReceivesButtonEvents, DrawPriority, Position>())
	{
		if (!PointIsInside(cursor_position, width_and_height, position)) { continue; }
		if (CursorIsOutsideScrollWindowContainingEntity(level, entity, cursor_position)) { continue; }
		entities_intersecting_mouse.push_back({ draw_priority->draw_priority, entity });
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
		for (auto& [entity, pressed] : level.GetEntitiesWith<Pressed>())
		{
			level.AddComponent<ReleasedThisFrame>(entity);
		}
		level.ClearComponent<Pressed>();
	}

	for (auto [entity, shortcut_key] : level.GetEntitiesWith<ShortcutKey>())
	{
		if (cursor_and_keys_.key_pressed_this_frame[shortcut_key->key])
		{
			cursor_and_keys_.key_pressed_this_frame[shortcut_key->key] = false;
			level.EnsureExistenceOfComponent<PressedThisFrame>(entity);
			level.EnsureExistenceOfComponent<Pressed>(entity); //When holding down, keys can act as being pressed multiple times without being released on between
		}
	}

	for (auto [entity, shortcut_key, pressed] : level.GetEntitiesWith<ShortcutKey, Pressed>())
	{
		if (cursor_and_keys_.key_released_this_frame[shortcut_key->key])
		{
			cursor_and_keys_.key_released_this_frame[shortcut_key->key] = false;
			level.AddComponent<ReleasedThisFrame>(entity);
			level.RemoveComponents<Pressed>(entity);
		}
	}

	std::vector<std::tuple<Entity, bool, bool, bool>> hovering_entities = {};

	std::vector<std::tuple<Entity, Entity>> entities_intersecting_mouse = {};
	entities_intersecting_mouse = GetEntitiesIntersectingMouse(level, cursor_and_keys_.cursor_position);

	auto entities_with_menu_navigator = level.GetEntitiesWith<MenuNavigator>();

	if (entities_intersecting_mouse.size() > 0)
	{
		Entity top_intersecting_id = std::get<1>(*max_element(entities_intersecting_mouse.begin(), entities_intersecting_mouse.end()));

		bool mouse_hover_inactive =
			entities_with_menu_navigator.size() > 0
			&& !cursor_and_keys_.cursor_moved_this_frame
			&& !cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left]
			&& !cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Left]
			&& level.HasComponents<MenuNavigable>(top_intersecting_id);

		if (!mouse_hover_inactive)
		{
			hovering_entities.push_back({ top_intersecting_id,
				cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left],
				cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Left],
				level.HasComponents<Hovered>(top_intersecting_id) });
		}
	}

	for (auto [entity, menu_navigator] : entities_with_menu_navigator)
	{
		if (!menu_navigator->current_snap_position.has_value()) { continue; }
		Entity at_id = menu_navigator->current_snap_position.value().entity;
		if (level.HasComponents<ReceivesButtonEvents>(at_id))
		{
			hovering_entities.push_back({ at_id,
				cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Enter],
				cursor_and_keys_.key_released_this_frame[sf::Keyboard::Enter],
				level.HasComponents<Hovered>(at_id) });
		}
	}

	level.ClearComponent<Hovered>();

	for (auto [entity, pressed_this_frame, released_this_frame, hovered_last_frame] : hovering_entities)
	{
		if (pressed_this_frame)
		{
			level.EnsureExistenceOfComponent<PressedThisFrame>(entity);
			level.EnsureExistenceOfComponent<Pressed>(entity);
		}
		if (released_this_frame)
		{
			level.EnsureExistenceOfComponent<ReleasedThisFrame>(entity);
			level.RemoveComponents<Pressed>(entity);
		}

		level.EnsureExistenceOfComponent<Hovered>(entity);
		if (!hovered_last_frame)
		{
			level.EnsureExistenceOfComponent<HoveredStartedThisFrame>(entity);
		}
	}
}