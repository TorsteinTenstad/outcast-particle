#include "_pure_DO_systems.hpp"
#include "level.hpp"

static int SnapToNextAbove(Level& level, MenuNavigator* menu_navigator, std::map<int, sf::Vector2f> possble_positions)
{
	for (auto it = possble_positions.rbegin(); it != possble_positions.rend(); ++it)
	{
		if (it->first < menu_navigator->currently_at_entity_id)
		{
			return it->first;
		}
	}
	return possble_positions.rbegin()->first;
}

static int SnapToNextBelow(Level& level, MenuNavigator* menu_navigator, std::map<int, sf::Vector2f> possble_positions)
{
	for (auto it = possble_positions.begin(); it != possble_positions.end(); ++it)
	{
		if (it->first > menu_navigator->currently_at_entity_id)
		{
			return it->first;
		}
	}
	return possble_positions.begin()->first;
}

static std::map<int, sf::Vector2f> GetPossiblePossiblePositions(Level& level)
{
	std::map<int, sf::Vector2f> possible_positions;
	for (auto [entity_id, menu_navigator, width_and_height, position] : level.GetEntitiesWith<MenuNavigatable, WidthAndHeight, Position>())
	{
		possible_positions[entity_id] = position->position;
		possible_positions[entity_id].x -= width_and_height->width_and_height.x / 2;
	}
	assert(possible_positions.size() > 0);
	return possible_positions;
}

void MenuNavigatonSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, menu_navigator, width_and_height, position] : level.GetEntitiesWith<MenuNavigator, WidthAndHeight, Position>())
	{
		int prev_at_id = menu_navigator->currently_at_entity_id;

		std::map<int, sf::Vector2f> possible_positions = GetPossiblePossiblePositions(level);

		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Down]
			|| menu_navigator->currently_at_entity_id < 0)
		{
			menu_navigator->currently_at_entity_id = SnapToNextBelow(level, menu_navigator, possible_positions);
		}
		else if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Up])
		{
			menu_navigator->currently_at_entity_id = SnapToNextAbove(level, menu_navigator, possible_positions);
		}
		else
		{
			for (auto [hovered_started_this_frame_id, hovered_started_this_frame, menu_navigatable, width_and_height, position] : level.GetEntitiesWith<HoveredStartedThisFrame, MenuNavigatable, WidthAndHeight, Position>())
			{
				menu_navigator->currently_at_entity_id = hovered_started_this_frame_id;
				break;
			}
		}
		assert(menu_navigator->currently_at_entity_id >= 0);

		level.GetComponent<Hovered>().clear();
		level.AddComponent<Hovered>(menu_navigator->currently_at_entity_id);
		position->position = possible_positions[menu_navigator->currently_at_entity_id];
		position->position.x -= width_and_height->width_and_height.x;
		if (menu_navigator->currently_at_entity_id != prev_at_id)
		{
			level.AddComponents<HoveredStartedThisFrame>(menu_navigator->currently_at_entity_id);
		}

		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Enter])
		{
			level.GetComponent<PressedThisFrame>()[menu_navigator->currently_at_entity_id];
		}
		if (cursor_and_keys_.key_released_this_frame[sf::Keyboard::Enter])
		{
			level.GetComponent<ReleasedThisFrame>()[menu_navigator->currently_at_entity_id];
		}
	}
}