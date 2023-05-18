#include "components/menu_navigator.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include <cassert>

static Entity SnapToNextAbove(Level& level, MenuNavigator* menu_navigator, std::map<Entity, sf::Vector2f> possible_positions)
{
	for (auto it = possible_positions.rbegin(); it != possible_positions.rend(); ++it)
	{
		if (it->first < menu_navigator->currently_at_entity)
		{
			return it->first;
		}
	}
	return possible_positions.rbegin()->first;
}

static Entity SnapToNextBelow(Level& level, MenuNavigator* menu_navigator, std::map<Entity, sf::Vector2f> possible_positions)
{
	for (auto it = possible_positions.begin(); it != possible_positions.end(); ++it)
	{
		if (it->first > menu_navigator->currently_at_entity)
		{
			return it->first;
		}
	}
	return possible_positions.begin()->first;
}

static std::map<Entity, sf::Vector2f> GetPossiblePossiblePositions(Level& level, MenuNavigator* menu_navigator)
{
	std::map<Entity, sf::Vector2f> possible_positions;
	if (menu_navigator->menu_items.has_value())
	{
		for (Entity entity : menu_navigator->menu_items.value())
		{
			auto [width_and_height, position] = level.GetComponents<WidthAndHeight, Position>(entity);

			possible_positions[entity] = position->position;
			possible_positions[entity].x -= width_and_height->width_and_height.x / 2;
		}
	}
	else
	{
		for (auto [entity, menu_navigator, width_and_height, position] : level.GetEntitiesWith<MenuNavigable, WidthAndHeight, Position>())
		{
			possible_positions[entity] = position->position;
			possible_positions[entity].x -= width_and_height->width_and_height.x / 2;
		}
	}
	assert(possible_positions.size() > 0);
	return possible_positions;
}

void MenuNavigatorSystem::Update(Level& level, float dt)
{
	auto navigators = level.GetEntitiesWith<MenuNavigator, DrawPriority, WidthAndHeight, Position>();
	if (navigators.size() == 0)
	{
		for (Entity entity : level.inactive_entities.GetEntitiesWithComponent<MenuNavigator>())
		{
			level.ActivateEntities(entity);
		}
		navigators = level.GetEntitiesWith<MenuNavigator, DrawPriority, WidthAndHeight, Position>();
	}
	if (navigators.size() == 0) { return; }
	for (auto [entity, menu_navigator, draw_priority, width_and_height, position] : navigators)
	{
		if (entity != std::get<Entity>(navigators.back()))
		{
			level.DeactivateEntities(entity);
			continue;
		};
		menu_navigator->moved_itself_this_frame = false;
		std::optional<Entity>& at_id = menu_navigator->currently_at_entity;
		std::map<Entity, sf::Vector2f> possible_positions = GetPossiblePossiblePositions(level, menu_navigator);

		if (cursor_and_keys_.key_pressed_this_frame[menu_navigator->increment_key]
			|| !at_id.has_value())
		{
			cursor_and_keys_.key_pressed_this_frame[menu_navigator->increment_key] = false;
			at_id = SnapToNextBelow(level, menu_navigator, possible_positions);
			menu_navigator->moved_itself_this_frame = true;
		}
		else if (cursor_and_keys_.key_pressed_this_frame[menu_navigator->decrement_key])
		{
			cursor_and_keys_.key_pressed_this_frame[menu_navigator->decrement_key] = false;
			at_id = SnapToNextAbove(level, menu_navigator, possible_positions);
			menu_navigator->moved_itself_this_frame = true;
		}
		else
		{
			for (auto [hovered_started_this_frame_id, hovered_started_this_frame, menu_navigable, width_and_height, position] : level.GetEntitiesWith<HoveredStartedThisFrame, MenuNavigable, WidthAndHeight, Position>())
			{
				at_id = hovered_started_this_frame_id;
				menu_navigator->moved_itself_this_frame = true;
				break;
			}
		}
		assert(at_id.has_value());

		position->position = possible_positions[at_id.value()];
		position->position.x -= width_and_height->width_and_height.x;
	}
}