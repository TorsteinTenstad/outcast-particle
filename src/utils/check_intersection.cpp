#include "utils/check_intersection.hpp"
#include "utils/math.hpp"

bool CheckIntersection(Position* position_a, Position* position_b, Radius* radius_a, Radius* radius_b)
{
	return Magnitude(position_b->position - position_a->position) < (radius_a->radius + radius_b->radius);
}

bool CheckIntersection(Position* position_a, Position* position_b, WidthAndHeight* width_and_height_a, WidthAndHeight* width_and_height_b)
{
	sf::Vector2f center_distance = Abs(position_a->position - position_b->position);
	sf::Vector2f average_size = (width_and_height_a->width_and_height + width_and_height_b->width_and_height) / 2.f;

	return center_distance.x < average_size.x && center_distance.y < average_size.y;
}

bool CheckIntersection(Position* circle_position, Position* square_position, Radius* radius, WidthAndHeight* width_and_height)
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

bool CheckIntersection(Position* square_position, Position* circle_position, WidthAndHeight* width_and_height, Radius* radius)
{
	return CheckIntersection(circle_position, square_position, radius, width_and_height);
}