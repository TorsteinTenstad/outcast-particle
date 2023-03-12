#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "controls_config.hpp"
#include "general_user_config.hpp"

struct Globals
{
	sf::RenderWindow render_window;
	float time = 0;
	float time_of_last_level_enter = 0;
	KeyConfig key_config;
	GeneralConfig general_config;
};

extern Globals& globals;
