#pragma once
#include "PCH.hpp"
#include <typeindex>
class Children
{
public:
	std::map<std::type_index, std::vector<int>> ids_owned_by_component;
};