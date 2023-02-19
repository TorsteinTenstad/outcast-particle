#pragma once
#include "components/physics.hpp"
#include "components/shader.hpp"
#include <vector>

class ScrollWindow
{
public:
	std::vector<int> entities;
	float scolled_distance = 0;
	float entity_height = 0;
	float bottom_margin = float(BLOCK_SIZE) / 2;
};