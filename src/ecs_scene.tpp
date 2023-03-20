#include "ecs_scene.hpp"
#include <cassert>

template <class Component>
std::map<int, Component>& ESCScene::GetComponentMap()
{
	if (components_.count(typeid(Component)) == 0)
	{
		std::map<int, Component> t_map;
		components_[typeid(Component)] = t_map;
	}
	return std::get<std::map<int, Component>>(components_.at(typeid(Component)));
}

template <class Component>
bool ESCScene::HasComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	return m.find(entity_id) != m.end();
}

template <class... Component>
bool ESCScene::HasComponents(int entity_id)
{
	return (HasComponent<Component>(entity_id) && ...);
}

template <class Component>
Component* ESCScene::EnsureExistanceOfComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	return &m[entity_id];
}

template <class... Component>
std::tuple<Component*...> ESCScene::EnsureExistanceOfComponents(int entity_id)
{
	return std::make_tuple(EnsureExistanceOfComponent<Component>(entity_id)...);
}

template <class Component>
Component* ESCScene::AddComponent(int entity_id)
{
	assert(!HasComponent<Component>(entity_id));
	return EnsureExistanceOfComponent<Component>(entity_id);
}

template <class Component>
Component* ESCScene::AddComponent(int entity_id, Component&& value)
{
	auto& m = GetComponentMap<Component>();
	assert(m.count(entity_id) == 0);
	m.emplace(entity_id, value);
	return &m.at(entity_id);
}

template <class... Component>
std::tuple<Component*...> ESCScene::AddComponents(int entity_id)
{
	return std::make_tuple(AddComponent<Component>(entity_id)...);
}

template <class Component>
Component* ESCScene::GetComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	auto it = m.find(entity_id);
	assert(it != m.end());
	return &(it->second);
}

template <class... Component>
std::tuple<Component*...> ESCScene::GetComponents(int entity_id)
{
	return std::make_tuple(GetComponent<Component>(entity_id)...);
}

template <class Component>
Component* ESCScene::RawGetComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	auto it = m.find(entity_id);
	if (it == m.end())
	{
		return nullptr;
	}
	return &(it->second);
}

template <class... Component>
std::tuple<Component*...> ESCScene::RawGetComponents(int entity_id)
{
	return std::make_tuple(RawGetComponent<Component>(entity_id)...);
}

template <class Component>
bool ESCScene::RemoveComponent(int entity_id)
{
	auto& m = GetComponentMap<Component>();
	if (Children* children = RawGetComponent<Children>(entity_id))
	{
		auto& it = children->ids_owned_by_component.find(typeid(Component));
		if (it != children->ids_owned_by_component.end())
		{
			for (int owned_entity : it->second)
			{
				DeleteEntity(owned_entity);
			}
		}
	}
	return m.erase(entity_id) > 0;
}

template <class... Component>
bool ESCScene::RemoveComponents(int entity_id)
{
	return (RemoveComponent<Component>(entity_id) || ...);
}

template <class... Component>
std::tuple<int, Component*...> ESCScene::CreateEntitiyWith()
{
	int entity_id = CreateEntityId();
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
std::vector<std::tuple<int, Component*...>> ESCScene::GetEntitiesWith()
{
	int component_idx = 0;
	std::vector<std::tuple<int, Component*...>> matching_entities = {};
	(IdIntersection<Component, Component...>(component_idx++, GetComponentMap<Component>(), matching_entities) && ...);
	return matching_entities;
}

template <class... Component>
void ESCScene::DeleteEntitiesWith()
{
	auto entities = GetEntitiesWith<Component...>();
	for (auto tup : entities)
	{
		int entiy_id = std::get<0>(tup);
		DeleteEntity(entiy_id);
	}
}

template <class Component>
std::tuple<int, Component*> ESCScene::GetSingletonIncludeID(std::function<void(ESCScene&, int)> creation_func)
{
	auto& component_map = GetComponentMap<Component>();

	if (component_map.size() == 0)
	{
		int entity_id = CreateEntityId();
		component_map[entity_id];
		creation_func(*this, entity_id);
	}
	assert(component_map.size() == 1);
	return { component_map.begin()->first, &component_map.begin()->second };
}

template <class Component>
std::tuple<int, Component*> ESCScene::GetSingletonIncludeID()
{
	return GetSingletonIncludeID<Component>([](ESCScene& scene, int) {});
}

template <class Component>
Component* ESCScene::GetSingleton(std::function<void(ESCScene&, int)> creation_func)
{
	std::tuple<int, Component*> tup = GetSingletonIncludeID<Component>(creation_func);
	return std::get<Component*>(tup);
}

template <class Component>
Component* ESCScene::GetSingleton()
{
	std::tuple<int, Component*> tup = GetSingletonIncludeID<Component>();
	return std::get<Component*>(tup);
}

template <class Component>
void ESCScene::ClearComponent()
{
	GetComponentMap<Component>().clear();
}