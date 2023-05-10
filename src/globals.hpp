#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "controls_config.hpp"
#include "general_user_config.hpp"

struct DeveloperOptions
{
	bool all_level_groups_are_editable = true; //Should be false in release
};

enum SoundTypes
{
	DEFAULT,
	ON_CLICK,
	TO_NEUTRAL,
	FROM_NEUTRAL
};

struct Globals
{
	sf::RenderWindow render_window;
	float time = 0;
	float time_of_last_level_enter = 0;
	KeyConfig key_config;
	GeneralConfig general_config;
	DeveloperOptions developer_options;
};

extern Globals& globals;
