#pragma once
#include "SFML/System/Vector2.hpp"
#include "constants.hpp"
#include <optional>

class Editable
{
public:
	float smallest_allowed_size = BLOCK_SIZE;
};

class Selected
{
public:
	sf::Vector2f mouse_offset;
};

class TemporarilyMoved
{
public:
	std::optional<sf::Vector2f> original_position;
};

class EditModeRectangleSelectTool
{};