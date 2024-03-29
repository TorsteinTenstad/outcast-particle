#pragma once
#include "SFML/System/Vector2.hpp"
#include <vector>

#define TRAIL_N 64

class Trail
{
public:
	std::vector<sf::Vector2f> path;
	std::vector<float> widths;
};