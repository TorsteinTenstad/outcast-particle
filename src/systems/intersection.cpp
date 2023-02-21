#include "components/intersection.hpp"
#include "_pure_DO_systems.hpp"
#include "components/physics.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

static bool CheckIntersection(Position* position_a, Position* position_b, Radius* radius_a, Radius* radius_b)
{
	return Magnitude(position_b->position - position_a->position) < (radius_a->radius + radius_b->radius);
}

static bool CheckIntersection(Position* circle_position, Position* square_position, Radius* radius, WidthAndHeight* width_and_height)
{
	sf::Vector2f center_distance = Abs(circle_position->position - square_position->position);

	if (center_distance.x > (width_and_height->width_and_height.x / 2 + radius->radius))
	{
		return false;
	}
	if (center_distance.y > (width_and_height->width_and_height.y / 2 + radius->radius))
	{
		return false;
	}

	if (center_distance.x <= (width_and_height->width_and_height.x / 2))
	{
		return true;
	}
	if (center_distance.y <= (width_and_height->width_and_height.y / 2))
	{
		return true;
	}
	float corner_distance = pow(center_distance.x - width_and_height->width_and_height.x / 2, 2) + pow(center_distance.y - width_and_height->width_and_height.y / 2, 2);
	return corner_distance <= pow(radius->radius, 2);
}

void IntersectionSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, intersection, radius, position] : level.GetEntitiesWith<Intersection, Radius, Position>())
	{
		assert(!level.HasComponents<WidthAndHeight>(entity_id));

		intersection->entered_this_frame_ids.clear();
		std::vector<int> previous_intersecting_ids = intersection->intersecting_ids;
		intersection->intersecting_ids.clear();

		for (auto const& [entity_id_b, radius_b, position_b] : level.GetEntitiesWith<Radius, Position>())
		{
			if (entity_id != entity_id_b)
			{
				if (CheckIntersection(position, position_b, radius, radius_b))
				{
					intersection->intersecting_ids.push_back(entity_id_b);
					if (std::count(previous_intersecting_ids.begin(), previous_intersecting_ids.end(), entity_id_b) == 0)
					{
						intersection->entered_this_frame_ids.push_back(entity_id_b);
					}
				}
			}
		}
		for (auto const& [entity_id_b, width_and_height, position_b] : level.GetEntitiesWith<WidthAndHeight, Position>())
		{
			if (CheckIntersection(position, position_b, radius, width_and_height))
			{
				intersection->intersecting_ids.push_back(entity_id_b);
				if (std::count(previous_intersecting_ids.begin(), previous_intersecting_ids.end(), entity_id_b) == 0)
				{
					intersection->entered_this_frame_ids.push_back(entity_id_b);
				}
			}
		}
	}
}
