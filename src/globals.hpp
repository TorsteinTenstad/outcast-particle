#pragma once

#include "controls_config.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

struct Globals
{
	sf::RenderWindow render_window;
	float time = 0;
	KeyConfig key_config;
};

extern Globals& globals;
