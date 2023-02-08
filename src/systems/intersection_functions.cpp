#include "systems/intersection_functions.hpp"
#include "components/area.hpp"
#include "components/physics.hpp"
#include "utils.hpp"

bool CheckIntersection(Position position_a, Position position_b, Radius radius_a, Radius radius_b)
{
	return Magnitude(position_b.position - position_a.position) < (radius_a.radius + radius_b.radius);
}

bool CheckIntersection(Position circle_position, Position square_position, Radius radius, WidthAndHeight width_and_height)
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