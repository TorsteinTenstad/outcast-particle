#pragma once

#include <typeindex>
class Children
{
public:
	std::map<std::type_index, std::vector<int>> ids_owned_by_component;
};