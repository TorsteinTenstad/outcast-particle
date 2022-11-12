#pragma once
#include "PCH.hpp"

class Editable
{
public:
	bool selected = false;
	bool is_charge_editable = false;
	bool is_velocity_editable = false;
	bool is_height_and_widht_editable = false;
	sf::Vector2f drag_offset;
};

class ClickedOn
{
public:
	bool clicked_on = false;
	bool clicked_this_frame = false;
	bool released_this_frame = false;
};