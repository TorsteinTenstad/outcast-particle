#pragma once
#include "SFML/Window/Mouse.hpp"
#include <optional>
#include <string>

class EditToolButton
{
public:
	std::string tool;
	bool is_drag_tool = true;
};

class ButtonBoundEditTool
{
public:
	sf::Mouse::Button button;
	std::optional<int> icon_entity {};
};