#pragma once
#include "level.hpp"

template <class Component>
void Level::RegisterComponent()
{
	std::map<int, Component> t_map;
	components_[typeid(Component)] = t_map;
}

template <class Component>
std::map<int, Component>& Level::GetComponent()
{
	if (components_.count(typeid(Component)) == 0)
	{
		RegisterComponent<Component>();
	}
	return std::get<std::map<int, Component>>(components_[typeid(Component)]);
}