#pragma once
#include "PCH.hpp"

class Draggable
{
public:
	bool being_dragged = false;
	sf::Vector2f offset;
};

class ClickedOn
{
public:
	bool clicked_on = false;
	bool clicked_this_frame = false;
	bool released_this_frame = false;
};