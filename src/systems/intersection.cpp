#include "components/intersection.hpp"
#include "_pure_DO_systems.hpp"
#include "components/physics.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils/math.hpp"

#include <cassert>

static bool CheckIntersection(Position* position_a, Position* position_b, Radius* radius_a, Radius* radius_b)
{
	return Magnitude(position_b->position - position_a->position) < (radius_a->radius + radius_b->radius);
}

static bool CheckIntersection(Position* position_a, Position* position_b, WidthAndHeight* width_and_height_a, WidthAndHeight* width_and_height_b)
{
	sf::Vector2f center_distance = Abs(position_a->position - position_b->position);
	sf::Vector2f average_size = (width_and_height_a->width_and_height + width_and_height_b->width_and_height) / 2.f;

	return center_distance.x < average_size.x && center_distance.y < average_size.y;
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

static bool CheckIntersection(Position* square_position, Position* circle_position, WidthAndHeight* width_and_height, Radius* radius)
{
	return CheckIntersection(circle_position, square_position, radius, width_and_height);
}

template <class SizeType, class OtherSizeType>
static void UpdateIntersectionForSizeTypePair(Intersection* intersection, std::vector<int> previous_intersecting_ids,
	int entity_id, int other_entity_id,
	Position* position, Position* other_position,
	SizeType size, OtherSizeType other_size)
{
	if (entity_id != other_entity_id)
	{
		if (CheckIntersection(position, other_position, size, other_size))
		{
			intersection->intersecting_ids.push_back(other_entity_id);
			if (std::count(previous_intersecting_ids.begin(), previous_intersecting_ids.end(), other_entity_id) == 0)
			{
				intersection->entered_this_frame_ids.push_back(other_entity_id);
			}
			intersection->left_this_frame_ids.erase(
				std::remove(
					intersection->left_this_frame_ids.begin(),
					intersection->left_this_frame_ids.end(),
					other_entity_id),
				intersection->left_this_frame_ids.end());
		}
	}
}

template <class SizeType>
static void UpdateIntersectionForEntitiesWith(Level& level)
{
	for (auto [entity_id, intersection, size, position] : level.GetEntitiesWith<Intersection, SizeType, Position>())
	{
		intersection->entered_this_frame_ids.clear();
		std::vector<int> previous_intersecting_ids = intersection->intersecting_ids;
		intersection->left_this_frame_ids.clear();
		intersection->intersecting_ids.swap(intersection->left_this_frame_ids);

		for (auto const& [other_entity_id, radius_b, other_position] : level.GetEntitiesWith<Radius, Position>())
		{
			UpdateIntersectionForSizeTypePair(intersection, previous_intersecting_ids, entity_id, other_entity_id, position, other_position, size, radius_b);
		}
		for (auto const& [other_entity_id, other_width_and_height, other_position] : level.GetEntitiesWith<WidthAndHeight, Position>())
		{
			UpdateIntersectionForSizeTypePair(intersection, previous_intersecting_ids, entity_id, other_entity_id, position, other_position, size, other_width_and_height);
		}
	}
}

void IntersectionSystem::Update(Level& level, float dt)
{
	bool no_conflicting_size_types = level.GetEntitiesWith<Radius, WidthAndHeight>().size() == 0;
	assert(no_conflicting_size_types);

	UpdateIntersectionForEntitiesWith<Radius>(level);
	UpdateIntersectionForEntitiesWith<WidthAndHeight>(level);
}
