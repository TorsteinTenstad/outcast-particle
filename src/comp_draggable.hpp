#pragma once
#include "PCH.hpp"

class Draggable
{
public:
	bool being_dragged = false;
	sf::Vector2f offset;
};