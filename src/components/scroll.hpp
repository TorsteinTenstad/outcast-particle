#pragma once
#include "components/physics.hpp"
#include "components/shader.hpp"
#include "constants.hpp"
#include "ecs/entity.hpp"
#include <optional>
#include <vector>

class ScrollWindow
{
public:
	std::vector<Entity> entities;
	float scrolled_distance = 0;
	float entity_height = 0;
	float bottom_margin = float(BLOCK_SIZE) / 2;
	std::optional<Entity> menu_navigator;
};