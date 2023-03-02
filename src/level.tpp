#include "level.hpp"
#include <cassert>

template <class Component>
std::map<int, Component>& Level::GetComponentMap()
{
	if (components_.count(typeid(Component)) == 0)
	{
		std::map<int, Component> t_map;
		components_[typeid(Component)] = t_map;
	}
	return std::get<std::map<int, Component>>(components_[typeid(Component)]);
}

template <class Component>
bool Level::HasComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	return m.find(entity_id) != m.end();
}

template <class... Component>
bool Level::HasComponents(int entity_id)
{
	return (HasComponent<Component>(entity_id) && ...);
}

template <class Component>
Component* Level::AddComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	return &m[entity_id];
}

template <class Component>
Component* Level::AddComponent(int entity_id, Component&& value)
{
	auto& m = GetComponentMap<Component>();
	assert(m.count(entity_id) == 0);
	m.emplace(entity_id, value);
	return &m.at(entity_id);
}

template <class... Component>
std::tuple<Component*...> Level::AddComponents(int entity_id)
{
	return std::make_tuple(AddComponent<Component>(entity_id)...);
}

template <class Component>
Component* Level::GetComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	return &m.at(entity_id);
}

template <class... Component>
std::tuple<Component*...> Level::GetComponents(int entity_id)
{
	return std::make_tuple(GetComponent<Component>(entity_id)...);
}

template <class Component>
bool Level::RemoveComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	return m.erase(entity_id) > 0;
}

template <class... Component>
bool Level::RemoveComponents(int entity_id)
{
	return (RemoveComponent<Component>(entity_id) || ...);
}

template <class... Component>
std::tuple<int, Component*...> Level::CreateEntitiyWith()
{
	int entity_id = CreateEntityId();
	return std::tuple_cat(std::make_tuple(entity_id), AddComponents<Component...>(entity_id));
}

template <class... Component>
std::tuple<int, Component*...> Level::AddBlueprintGetComponents(std::string tag)
{
	int entity_id = AddBlueprint(tag);
	return std::tuple_cat(std::make_tuple(entity_id), GetComponents<Component...>(entity_id));
}

template <class... Component>
std::tuple<int, Component*...> Level::AddBlueprintAddComponents(std::string tag)
{
	int entity_id = AddBlueprint(tag);
	return std::tuple_cat(std::make_tuple(entity_id), AddComponents<Component...>(entity_id));
}

template <class OtherComponent, class... Component>
static bool IdIntersection(int component_idx, std::map<int, OtherComponent>& component_map, std::vector<std::tuple<int, Component*...>>& matching_entities)
{
	if (component_map.size() == 0)
	{
		matching_entities.clear();
		return false;
	}
	if (component_idx == 0)
	{
		for (auto& [entity_id, component_value] : component_map)
		{
			std::tuple<int, Component*...> x = {};
			std::get<int>(x) = entity_id;
			std::get<OtherComponent*>(x) = &component_value;
			matching_entities.push_back(x);
		}
	}
	else
	{
		for (auto it = matching_entities.begin(); it != matching_entities.end();)
		{
			auto& map_it = component_map.find(std::get<int>(*it));
			if (map_it == component_map.end())
			{
				it = matching_entities.erase(it);
			}
			else
			{
				std::get<OtherComponent*>(*it) = &map_it->second;
				it++;
			}
		}
	}
	return true;
}

template <class... Component>
std::vector<std::tuple<int, Component*...>> Level::GetEntitiesWith()
{
	int component_idx = 0;
	std::vector<std::tuple<int, Component*...>> matching_entities = {};
	(IdIntersection<Component, Component...>(component_idx++, GetComponentMap<Component>(), matching_entities) && ...);
	return matching_entities;
}

template <class... Component>
void Level::DeleteEntitiesWith()
{
	auto entities = GetEntitiesWith<Component...>();
	for (auto tup : entities)
	{
		int entiy_id = std::get<0>(tup);
		DeleteEntity(entiy_id);
	}
}

template <class Component>
std::tuple<int, Component*> GetSingletonIncludeID(Level& level)
{
	auto& component_map = level.GetComponentMap<Component>();
	assert(!(component_map.size() > 1));

	if (component_map.size() == 0)
	{
		int entity_id = level.CreateEntityId();
		component_map[entity_id];
	}
	return { component_map.begin()->first, &component_map.begin()->second };
}

template <class Component>
Component* GetSingleton(Level& level)
{
	std::tuple<int, Component*> tup = GetSingletonIncludeID<Component>(level);
	return std::get<Component*>(tup);
}

template <class Component>
void Level::ClearComponent()
{
	GetComponentMap<Component>().clear();
}

template <class ResponsibleComponent>
int EnsureExistanceOfChildEntity(Children* parents_children, std::function<int(void)> child_creation_func)
{
	if (parents_children->ids_owned_by_component.count(typeid(ResponsibleComponent)) == 0)
	{
		int child_id = child_creation_func();
		parents_children->ids_owned_by_component[typeid(ResponsibleComponent)].push_back(child_id);
	}
	return parents_children->ids_owned_by_component[typeid(ResponsibleComponent)][0];
}

template <class ResponsibleComponent>
Shader* EnsureExistanceOfScreenwideFragmentShaderChildEntity(Level& level, Children* parents_children, std::string shader_path, int draw_priority)
{
	std::function<int(void)> child_creation_func = [&]() { return CreateScreenwideFragmentShaderEntity(level, shader_path, draw_priority); };
	int child_id = EnsureExistanceOfChildEntity<ResponsibleComponent>(parents_children, child_creation_func);
	return level.GetComponent<Shader>(child_id);
}