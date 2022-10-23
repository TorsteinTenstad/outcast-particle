#pragma once

//Function to take in two position vectors, returns the distance between the positions.
static float Distance(sf::Vector2f(position_1), sf::Vector2f(position_2))
{
	sf::Vector2f distance_vector = position_1 - position_2;
	float distance = std::sqrt(distance_vector.x * distance_vector.x + distance_vector.y * distance_vector.y);
	return (distance);
}
