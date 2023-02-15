#pragma once
#include "SFML/Window/Mouse.hpp"
#include <string>

class EditToolButton
{
public:
	std::string tool;
};

class ButtonBoundEditTool
{
public:
	sf::Mouse::Button button;
	std::string tool;
};