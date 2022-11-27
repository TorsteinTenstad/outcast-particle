#pragma once

#include "PCH.hpp"

#define BLOCK_SIZE 120
#define MAX_SCREEN_SIZE_SHAKE 1.1f

const std::array<float, 10> PARTICLE_CHARGE_CATEGORIES = { -1000000, -100000, -10000, -1000, -100, 100, 1000, 10000, 100000, 1000000 };
const std::array<std::string, 10> PLAYER_PARTICLE_TEXTURES = {
	"content\\textures\\particle_-5.png",
	"content\\textures\\particle_-4.png",
	"content\\textures\\particle_-3.png",
	"content\\textures\\particle_-2.png",
	"content\\textures\\particle_-1.png",
	"content\\textures\\particle_1.png",
	"content\\textures\\particle_2.png",
	"content\\textures\\particle_3.png",
	"content\\textures\\particle_4.png",
	"content\\textures\\particle_5.png",
};
const std::array<std::string, 10> PARTICLE_TEXTURES = {
	"content\\textures\\particle_-5.png",
	"content\\textures\\particle_-4.png",
	"content\\textures\\particle_-3.png",
	"content\\textures\\particle_-2.png",
	"content\\textures\\particle_-1.png",
	"content\\textures\\particle_1.png",
	"content\\textures\\particle_2.png",
	"content\\textures\\particle_3.png",
	"content\\textures\\particle_4.png",
	"content\\textures\\particle_5.png",
};