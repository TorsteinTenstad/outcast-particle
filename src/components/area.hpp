#pragma once

class Radius
{
public:
	float radius;
};

class WidthAndHeight
{
public:
	sf::Vector2f width_and_height;
};

class Border
{
public:
	int thickness = 20;
	sf::Color color = sf::Color::White;
};