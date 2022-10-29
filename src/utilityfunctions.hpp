#pragma once

#define PI 3.141593

static float Magnitude(sf::Vector2f v)
{
	return std::sqrt((v.x * v.x) + (v.y * v.y));
}
