#pragma once
#include <array>

#define MAIN_MENU "_MAIN_MENU"
#define LEVEL_MENU "_LEVEL_MENU"
#define OPTIONS_MENU "_OPTIONS_MENU"

#define STARTING_LEVEL MAIN_MENU

#define BLOCK_SIZE 120
#define MAX_SCREEN_SIZE_SHAKE 1.1f
#define BACKGROUND_DRAW_PRIORITY -1
#define UI_BASE_DRAW_PRIORITY 100
#define EDIT_MODE_PANEL_WIDTH (4 * BLOCK_SIZE)

#define DEFAULT_LEVEL_GRID_SIZE_ID 1

const std::array<float, 6> CHARGE_CATEGORY_KEYS = { sf::Keyboard::Num5, sf::Keyboard::Num4, sf::Keyboard::Num3, sf::Keyboard::Num2, sf::Keyboard::Num1, sf::Keyboard::Num0 };
const std::array<float, 11> PARTICLE_CHARGE_CATEGORIES = { -160'000, -40'000, -10'000, -2500, -625, 0, 625, 2500, 10'000, 40'000, 160'000 };
const std::array<std::string, 11> PARTICLE_TEXTURES = {
	"content\\textures\\particle_green_-5.png",
	"content\\textures\\particle_green_-4.png",
	"content\\textures\\particle_green_-3.png",
	"content\\textures\\particle_green_-2.png",
	"content\\textures\\particle_green_-1.png",
	"content\\textures\\particle_blue_0.png",
	"content\\textures\\particle_red_+1.png",
	"content\\textures\\particle_red_+2.png",
	"content\\textures\\particle_red_+3.png",
	"content\\textures\\particle_red_+4.png",
	"content\\textures\\particle_red_+5.png"
};
const std::array<std::string, 11> PLAYER_PARTICLE_TEXTURES = {
	"content\\textures_generated\\particle_green_5.png",
	"content\\textures_generated\\particle_green_4.png",
	"content\\textures_generated\\particle_green_3.png",
	"content\\textures_generated\\particle_green_2.png",
	"content\\textures_generated\\particle_green_1.png",
	"content\\textures_generated\\particle_blue_0.png",
	"content\\textures_generated\\particle_red_1.png",
	"content\\textures_generated\\particle_red_2.png",
	"content\\textures_generated\\particle_red_3.png",
	"content\\textures_generated\\particle_red_4.png",
	"content\\textures_generated\\particle_red_5.png"
};
const std::array<std::string, 11> PLAYER_PARTICLE_NEUTRAL_TEXTURES = {
	"content\\textures_generated\\particle_blue_5.png",
	"content\\textures_generated\\particle_blue_4.png",
	"content\\textures_generated\\particle_blue_3.png",
	"content\\textures_generated\\particle_blue_2.png",
	"content\\textures_generated\\particle_blue_1.png",
	"content\\textures_generated\\particle_blue_0.png",
	"content\\textures_generated\\particle_blue_1.png",
	"content\\textures_generated\\particle_blue_2.png",
	"content\\textures_generated\\particle_blue_3.png",
	"content\\textures_generated\\particle_blue_4.png",
	"content\\textures_generated\\particle_blue_5.png"
};

const std::array<float, 5> ELECTRIC_FIELD_STRENGTH_CATEGORIES = { 0.0625, 0.125, 0.25, 0.5, 1 };
const std::array<float, 5> ELECTRIC_FIELD_CATEGORY_KEYS = { sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4, sf::Keyboard::Num5 };
const std::array<std::string, 5> ELECTRIC_FIELD_TEXTURES = {
	"content\\textures\\electric_field_1.png",
	"content\\textures\\electric_field_2.png",
	"content\\textures\\electric_field_3.png",
	"content\\textures\\electric_field_4.png",
	"content\\textures\\electric_field_5.png"
};
const std::array<float, 10> MAGNETIC_FIELD_STRENGTH_CATEGORIES = { -0.8, -0.4, -0.2, -0.1, -0.05, 0.05, 0.1, 0.2, 0.4, 0.8 };
const std::array<float, 5> MAGNETIC_FIELD_CATEGORY_KEYS = { sf::Keyboard::Num5, sf::Keyboard::Num4, sf::Keyboard::Num3, sf::Keyboard::Num2, sf::Keyboard::Num1 };
const std::array<std::string, 10> MAGNETIC_FIELD_TEXTURES = {
	"content\\textures_generated\\magnetic_field_x5.png",
	"content\\textures_generated\\magnetic_field_x4.png",
	"content\\textures_generated\\magnetic_field_x3.png",
	"content\\textures_generated\\magnetic_field_x2.png",
	"content\\textures_generated\\magnetic_field_x1.png",
	"content\\textures_generated\\magnetic_field_o1.png",
	"content\\textures_generated\\magnetic_field_o2.png",
	"content\\textures_generated\\magnetic_field_o3.png",
	"content\\textures_generated\\magnetic_field_o4.png",
	"content\\textures_generated\\magnetic_field_o5.png"
};