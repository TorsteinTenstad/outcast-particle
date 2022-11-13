#pragma once
#include "PCH.hpp"

class Collision
{
public:
	float bounce_factor = 1;
	sf::Vector2f last_frame_position = sf::Vector2f(0, 0);
};