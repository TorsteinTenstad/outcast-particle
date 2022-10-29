#pragma once
#include "PCH.hpp"

struct Globals
{
	sf::RenderWindow render_window;
	int active_level;
};

extern Globals& globals;
