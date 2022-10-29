#pragma once
#include "PCH.hpp"

struct Globals
{
	sf::RenderWindow render_window;
	int active_level = 0;
	bool edit_mode = false;
};

extern Globals& globals;
