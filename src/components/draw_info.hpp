#pragma once
#include <string>

class DrawInfo
{
public:
	std::string image_path = "_";
	bool scale_to_fit = false;
	int quarter_turn_rotations = 0;
};

class DrawPriority
{
public:
	int draw_priority = 1;
};

class FillColor
{
public:
	sf::Color color = sf::Color::White;
};

class MouseInteractionDependentFillColor
{
public:
	sf::Color hovered_color = sf::Color::White;
	sf::Color pressed_color = sf::Color::White;
	sf::Color default_color = sf::Color::White;
};

class ChargeDependentDrawInfo
{
};

class OrientationDependentDrawInfo
{
};

class Background
{
};

class Face
{
public:
	std::string image_path;
};