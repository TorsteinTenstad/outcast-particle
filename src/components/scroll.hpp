#pragma once
#include "components/physics.hpp"
#include "components/shader.hpp"
#include "constants.hpp"
#include <optional>
#include <vector>

class ScrollWindow
{
public:
	std::vector<int> entities;
	float scrolled_distance = 0;
	float entity_height = 0;
	float bottom_margin = float(BLOCK_SIZE) / 2;
	std::optional<int> menu_navigator;
};