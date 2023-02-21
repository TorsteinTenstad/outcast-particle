#pragma once

#define TRAIL_N 64

class Trail
{
public:
	std::vector<sf::Vector2f> path;
	std::vector<float> widths;
};