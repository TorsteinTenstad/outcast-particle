#pragma once
#include "ecs/entity.hpp"
#include <vector>

class Intersection
{
public:
	std::vector<Entity> intersecting_entities;
	std::vector<Entity> entities_entered_this_frame;
	std::vector<Entity> entities_left_this_frame;
};