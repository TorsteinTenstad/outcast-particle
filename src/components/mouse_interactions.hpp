#pragma once

#include "globals.hpp"

class Editable
{
public:
	bool is_position_editable = true;
	bool is_charge_editable = false;
	bool is_velocity_editable = false;
	float width_and_height_edit = 0;
	sf::Vector2f drag_offset;
	bool selected = false;
};

class ClickedOn
{
public:
	bool clicked_on = false;
	bool clicked_this_frame = false;
	bool released_this_frame = false;
};