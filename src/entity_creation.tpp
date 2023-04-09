#include "level.hpp"
#include <cassert>

template <class ResponsibleComponent>
int EnsureExistenceOfChildEntity(Children* parents_children, std::function<int(void)> child_creation_func)
{
	if (parents_children->ids_owned_by_component.count(typeid(ResponsibleComponent)) == 0)
	{
		int child_id = child_creation_func();
		parents_children->ids_owned_by_component[typeid(ResponsibleComponent)].push_back(child_id);
	}
	assert(parents_children->ids_owned_by_component[typeid(ResponsibleComponent)].size() == 1);
	return parents_children->ids_owned_by_component[typeid(ResponsibleComponent)][0];
}

template <class ResponsibleComponent>
Shader* EnsureExistenceOfScreenWideFragmentShaderChildEntity(Level& level, Children* parents_children, std::string shader_path, int draw_priority)
{
	std::function<int(void)> child_creation_func = [&]() { return CreateScreenWideFragmentShaderEntity(level, shader_path, draw_priority); };
	int child_id = EnsureExistenceOfChildEntity<ResponsibleComponent>(parents_children, child_creation_func);
	return level.GetComponent<Shader>(child_id);
}