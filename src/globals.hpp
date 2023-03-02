#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "controls_config.hpp"

struct Globals
{
	sf::RenderWindow render_window;
	float time = 0;
	KeyConfig key_config;
};

extern Globals& globals;
