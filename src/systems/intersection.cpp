#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
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
static void UpdateIntersectionForSizeTypePair(Intersection* intersection, std::vector<Entity> previous_intersecting_entities,
	Entity entity, Entity other_entity,
	Position* position, Position* other_position,
	SizeType size, OtherSizeType other_size)
{
	if (entity != other_entity)
	{
		if (CheckIntersection(position, other_position, size, other_size))
		{
			intersection->intersecting_entities.push_back(other_entity);
			if (std::count(previous_intersecting_entities.begin(), previous_intersecting_entities.end(), other_entity) == 0)
			{
				intersection->entities_entered_this_frame.push_back(other_entity);
			}
			intersection->entities_left_this_frame.erase(
				std::remove(
					intersection->entities_left_this_frame.begin(),
					intersection->entities_left_this_frame.end(),
					other_entity),
				intersection->entities_left_this_frame.end());
		}
	}
}

template <class SizeType>
static void UpdateIntersectionForEntitiesWith(Level& level)
{
	for (auto [entity, intersection, size, position] : level.GetEntitiesWith<Intersection, SizeType, Position>())
	{
		intersection->entities_entered_this_frame.clear();
		std::vector<Entity> previous_intersecting_entities = intersection->intersecting_entities;
		intersection->entities_left_this_frame.clear();
		intersection->intersecting_entities.swap(intersection->entities_left_this_frame);

		for (auto const& [other_entity, radius_b, other_position] : level.GetEntitiesWith<Radius, Position>())
		{
			UpdateIntersectionForSizeTypePair(intersection, previous_intersecting_entities, entity, other_entity, position, other_position, size, radius_b);
		}
		for (auto const& [other_entity, other_width_and_height, other_position] : level.GetEntitiesWith<WidthAndHeight, Position>())
		{
			UpdateIntersectionForSizeTypePair(intersection, previous_intersecting_entities, entity, other_entity, position, other_position, size, other_width_and_height);
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
