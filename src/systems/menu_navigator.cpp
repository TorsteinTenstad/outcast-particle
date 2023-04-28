#include "components/menu_navigator.hpp"
#include "components/button_events.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include <cassert>

static int SnapToNextAbove(Level& level, MenuNavigator* menu_navigator, std::map<int, sf::Vector2f> possible_positions)
{
	for (auto it = possible_positions.rbegin(); it != possible_positions.rend(); ++it)
	{
		if (it->first < menu_navigator->currently_at_entity_id)
		{
			return it->first;
		}
	}
	return possible_positions.rbegin()->first;
}

static int SnapToNextBelow(Level& level, MenuNavigator* menu_navigator, std::map<int, sf::Vector2f> possible_positions)
{
	for (auto it = possible_positions.begin(); it != possible_positions.end(); ++it)
	{
		if (it->first > menu_navigator->currently_at_entity_id)
		{
			return it->first;
		}
	}
	return possible_positions.begin()->first;
}

static std::map<int, sf::Vector2f> GetPossiblePossiblePositions(Level& level)
{
	std::map<int, sf::Vector2f> possible_positions;
	for (auto [entity_id, menu_navigator, width_and_height, position] : level.GetEntitiesWith<MenuNavigable, WidthAndHeight, Position>())
	{
		possible_positions[entity_id] = position->position;
		possible_positions[entity_id].x -= width_and_height->width_and_height.x / 2;
	}
	assert(possible_positions.size() > 0);
	return possible_positions;
}

void MenuNavigatorSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, menu_navigator, width_and_height, position] : level.GetEntitiesWith<MenuNavigator, WidthAndHeight, Position>())
	{
		menu_navigator->moved_itself_this_frame = false;
		std::optional<int>& at_id = menu_navigator->currently_at_entity_id;
		std::map<int, sf::Vector2f> possible_positions = GetPossiblePossiblePositions(level);

		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Down]
			|| !at_id.has_value())
		{
			at_id = SnapToNextBelow(level, menu_navigator, possible_positions);
			menu_navigator->moved_itself_this_frame = true;
		}
		else if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Up])
		{
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