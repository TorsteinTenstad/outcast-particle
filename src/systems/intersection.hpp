#pragma once
#include "PCH.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

static bool CheckIntersection(Position position_a, Position position_b, Radius radius_a, Radius radius_b)
{
	return Magnitude(position_b.position - position_a.position) < (radius_a.radius + radius_b.radius);
}

static bool CheckIntersection(Position circle_position, Position square_position, Radius radius, WidthAndHeight width_and_height)
{
	sf::Vector2f center_distance = Abs(circle_position.position - square_position.position);

	if (center_distance.x > (width_and_height.width_and_height.x / 2 + radius.radius))
	{
		return false;
	}
	if (center_distance.y > (width_and_height.width_and_height.y / 2 + radius.radius))
	{
		return false;
	}

	if (center_distance.x <= (width_and_height.width_and_height.x / 2))
	{
		return true;
	}
	if (center_distance.y <= (width_and_height.width_and_height.y / 2))
	{
		return true;
	}
	float corner_distance = pow(center_distance.x - width_and_height.width_and_height.x / 2, 2) + pow(center_distance.y - width_and_height.width_and_height.y / 2, 2);
	return corner_distance <= pow(radius.radius, 2);
}

class IntersectionSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, Radius>& radius_map = level.GetComponent<Radius>();
		std::map<int, WidthAndHeight>& width_and_height_map = level.GetComponent<WidthAndHeight>();
		std::map<int, Intersection>& intersection_map = level.GetComponent<Intersection>();
		for (auto& [entity_id, intersection] : intersection_map)
		{
			assert(radius_map.count(entity_id) > 0);
			assert(width_and_height_map.count(entity_id) == 0);

			intersection.intersecting_ids.clear();
			for (auto const& [entity_id_b, entity_radius_b] : radius_map)
			{
				if (entity_id != entity_id_b)
				{
					if (CheckIntersection(position_map[entity_id], position_map[entity_id_b], radius_map[entity_id], entity_radius_b))
					{
						intersection.intersecting_ids.push_back(entity_id_b);
					}
				}
			}
			for (auto const& [entity_id_b, entity_width_and_height] : width_and_height_map)
			{
				if (CheckIntersection(position_map[entity_id], position_map[entity_id_b], radius_map[entity_id], entity_width_and_height))
				{
					intersection.intersecting_ids.push_back(entity_id_b);
				}
			}
		}
	}
};