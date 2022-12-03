#pragma once
#include "PCH.hpp"

struct Globals
{
	sf::RenderWindow render_window;
	float time = 0;
};

extern Globals& globals;
