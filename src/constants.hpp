#pragma once

#include "PCH.hpp"

#define BLOCK_SIZE 120
#define MAX_SCREEN_SIZE_SHAKE 1.1f

const std::array<float, 11> PARTICLE_CHARGE_CATEGORIES = { -1000000, -100000, -10000, -1000, -100, 0, 100, 1000, 10000, 100000, 1000000 };
const std::array<std::string, 11> PLAYER_PARTICLE_TEXTURES = {
	"content\\textures\\particle_green_-5.png",
	"content\\textures\\particle_green_-4.png",
	"content\\textures\\particle_green_-3.png",
	"content\\textures\\particle_green_-2.png",
	"content\\textures\\particle_green_-1.png",
	"content\\textures\\particle_green_0.png",
	"content\\textures\\particle_red_+1.png",
	"content\\textures\\particle_red_+2.png",
	"content\\textures\\particle_red_+3.png",
	"content\\textures\\particle_red_+4.png",
	"content\\textures\\particle_red_+5.png",
};
const std::array<std::string, 11> PARTICLE_TEXTURES = {
	"content\\textures\\particle_blue_-5.png",
	"content\\textures\\particle_blue_-4.png",
	"content\\textures\\particle_blue_-3.png",
	"content\\textures\\particle_blue_-2.png",
	"content\\textures\\particle_blue_-1.png",
	"content\\textures\\particle_blue_0.png",
	"content\\textures\\particle_blue_+1.png",
	"content\\textures\\particle_blue_+2.png",
	"content\\textures\\particle_blue_+3.png",
	"content\\textures\\particle_blue_+4.png",
	"content\\textures\\particle_blue_+5.png",
};

const std::array<float, 5> ELECTRIC_FIELD_STRENGTH_CATEGORIES = { 5, 10, 25, 40, 60 };
const std::array<std::string, 5> ELECTRIC_FIELD_TEXTURES = {
	"content\\textures\\electric_field_1.png",
	"content\\textures\\electric_field_2.png",
	"content\\textures\\electric_field_3.png",
	"content\\textures\\electric_field_4.png",
	"content\\textures\\electric_field_5.png",
};