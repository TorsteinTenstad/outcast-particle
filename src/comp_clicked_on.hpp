#pragma once
#include "PCH.hpp"

class ClickedOn
{
public:
	bool clicked_on = false;
};

class Draggable
{
public:
	bool being_dragged = false;
	sf::Vector2f offset;
};