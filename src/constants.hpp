#pragma once
#include "SFML/Graphics/Color.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <string>
#include <vector>

#define MAIN_MENU "_MAIN_MENU"
#define LEVEL_MENU "_LEVEL_MENU"
#define OPTIONS_MENU "_OPTIONS_MENU"
#define KEY_CONFIG_MENU "_KEY_CONFIG_MENU"
#define GRAPHICS_AND_DISPLAY_MENU "_GRAPHICS_AND_DISPLAY_MENU"
#define MUSIC_AND_SOUND_MENU "_MUSIC_AND_SOUND_MENU"

#define STARTING_LEVEL MAIN_MENU
#define BLOCK_SIZE 120
#define MAX_SCREEN_SIZE_SHAKE 1.1f
#define BACKGROUND_DRAW_PRIORITY -1
#define UI_BASE_DRAW_PRIORITY 100
#define GLOBAL_MAX_ACCELERATION 10000

const std::vector<float> PARTICLE_CHARGE_CATEGORIES = {
	625,
	2500,
	10'000,
	40'000,
	160'000,
};

const std::vector<float> ELECTRIC_FIELD_STRENGTH_CATEGORIES = {
	0.0625,
	0.125,
	0.25,
	0.5,
	1
};

const std::vector<float> MAGNETIC_FIELD_STRENGTH_CATEGORIES = {
	0.05,
	0.1,
	0.2,
	0.4,
	0.8,
};

const std::vector<float> WALL_BOUNCE_CATEGORIES = {
	0.0,
	0.05,
	0.2,
	1.0,
	1.75,
};

const std::vector<std::string> WALL_SOUND_CATEGORIES = {
	"content\\sounds\\wall_1.wav",
	"content\\sounds\\wall_2.wav",
	"content\\sounds\\wall_3.wav",
	"content\\sounds\\wall_4.wav",
	"content\\sounds\\wall_5.wav"
};

const std::vector<sf::Keyboard::Key> CATEGORY_KEYS = {
	sf::Keyboard::Num1,
	sf::Keyboard::Num2,
	sf::Keyboard::Num3,
	sf::Keyboard::Num4,
	sf::Keyboard::Num5,
};