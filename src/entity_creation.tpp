#include "level.hpp"
#include <cassert>

template <class ResponsibleComponent>
int GetSingletonChildId(Level& level, int parent_id, std::function<int(Level&)> child_creation_func)
{
	Children* children = level.EnsureExistenceOfComponent<Children>(parent_id);
	if (children->ids_owned_by_component.count(typeid(ResponsibleComponent)) == 0)
	{
		int child_id = child_creation_func(level);
		children->ids_owned_by_component[typeid(ResponsibleComponent)].push_back(child_id);
	}
	assert(children->ids_owned_by_component[typeid(ResponsibleComponent)].size() == 1);
	return children->ids_owned_by_component[typeid(ResponsibleComponent)][0];
}

template <class ResponsibleComponent>
Shader* GetSingletonScreenWideFragmentShaderChildId(Level& level, int parent_id, std::string shader_path, int draw_priority)
{
	std::function<int(Level&)> child_creation_func = [shader_path, draw_priority](Level& level) { return CreateScreenWideFragmentShaderEntity(level, shader_path, draw_priority); };
	int child_id = GetSingletonChildId<ResponsibleComponent>(level, parent_id, child_creation_func);
	return level.GetComponent<Shader>(child_id);
}