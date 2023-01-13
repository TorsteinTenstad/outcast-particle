#pragma once
#include <functional>
#include <SFML/System/Vector2.hpp>

class AnimatedPosition
{
public:
	float start_time = 0;
	std::function<sf::Vector2f(float)> position_at_time_func;
};
