#pragma once
#include "SFML/Graphics/Color.hpp"
#include <optional>
#include <string>

const sf::Color HOVERED_COLOR = sf::Color(200, 200, 200);
const sf::Color PRESSED_COLOR = sf::Color(120, 120, 120);
const sf::Color DEFAULT_COLOR = sf::Color(150, 150, 150);

class DrawInfo
{
public:
	std::optional<std::string> image_path = std::nullopt;
	bool tile = false;
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
	sf::Color hovered_color = HOVERED_COLOR;
	sf::Color pressed_color = PRESSED_COLOR;
	sf::Color default_color = DEFAULT_COLOR;
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