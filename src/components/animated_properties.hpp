#pragma once
#include "SFML/Config.hpp"
#include <SFML/System/Vector2.hpp>
#include <functional>

class AnimatedPosition
{
public:
	float start_time = 0;
	std::function<sf::Vector2f(float)> animation_func;
};

class AnimatedOpacity
{
public:
	float start_time = 0;
	std::function<sf::Uint8(float)> animation_func;
};
