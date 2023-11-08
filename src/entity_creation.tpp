#include "level.hpp"
#include <cassert>

template <class ResponsibleComponent>
Entity GetSingletonChildId(Level& level, Entity parent, std::function<Entity(Level&)> child_creation_func)
{
	Children* children = level.EnsureExistenceOfComponent<Children>(parent);
	if (children->entities_owned_by_component.count(typeid(ResponsibleComponent)) == 0)
	{
		Entity child = child_creation_func(level);
		children->entities_owned_by_component[typeid(ResponsibleComponent)].push_back(child);
	}
	assert(children->entities_owned_by_component[typeid(ResponsibleComponent)].size() == 1);
	return children->entities_owned_by_component[typeid(ResponsibleComponent)][0];
}

template <class ResponsibleComponent>
Entity GetSingletonScreenWideFragmentShaderChildId(Level& level, Entity parent, std::string shader_path, int draw_priority)
{
	std::function<Entity(Level&)> child_creation_func = [shader_path, draw_priority](Level& level) { return CreateScreenWideFragmentShaderEntity(level, shader_path, draw_priority); };
	return GetSingletonChildId<ResponsibleComponent>(level, parent, child_creation_func);
}

template <class ResponsibleComponent>
void DeleteChildrenOwnedBy(Level& level, Entity parent)
{
	if (Children* children = level.RawGetComponent<Children>(parent))
	{
		auto it = children->entities_owned_by_component.find(typeid(ResponsibleComponent));
		if (it == children->entities_owned_by_component.end()) { return; }
		for (Entity child : it->second)
		{
			level.DeleteEntity(child);
		}
		children->entities_owned_by_component.erase(typeid(ResponsibleComponent));
	}
}