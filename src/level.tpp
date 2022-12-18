#include "level.hpp"

template <class Component>
std::map<int, Component>& Level::GetComponent()
{
	if (components_.count(typeid(Component)) == 0)
	{
		std::map<int, Component> t_map;
		components_[typeid(Component)] = t_map;
	}
	return std::get<std::map<int, Component>>(components_[typeid(Component)]);
}