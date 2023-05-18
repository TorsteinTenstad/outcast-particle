#pragma once

#include "ecs/entity.hpp"
#include <map>
#include <typeindex>
#include <vector>

class Children
{
public:
	std::map<std::type_index, std::vector<Entity>> entities_owned_by_component;
};