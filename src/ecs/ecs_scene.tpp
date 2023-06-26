#include "ecs/ecs_scene.hpp"
#include <cassert>

template <class Component>
bool ECSScene::HasComponent(Entity entity)
{
	auto& m = entity_container_.GetComponentMap<Component>();
	return m.find(entity) != m.end();
}

template <class... Component>
bool ECSScene::HasComponents(Entity entity)
{
	return (HasComponent<Component>(entity) && ...);
}

template <class Component>
Component* ECSScene::EnsureExistenceOfComponent(Entity entity)
{
	auto& m = entity_container_.GetComponentMap<Component>();
	return &m[entity];
}

template <class... Component>
std::tuple<Component*...> ECSScene::EnsureExistenceOfComponents(Entity entity)
{
	return std::make_tuple(EnsureExistenceOfComponent<Component>(entity)...);
}

template <class Component>
Component* ECSScene::AddComponent(Entity entity)
{
	assert(!HasComponent<Component>(entity));
	return EnsureExistenceOfComponent<Component>(entity);
}

template <class Component>
Component* ECSScene::AddComponent(Entity entity, Component&& value)
{
	auto& m = entity_container_.GetComponentMap<Component>();
	assert(m.count(entity) == 0);
	m.emplace(entity, std::move(value));
	return &m.at(entity);
}

template <class... Component>
std::tuple<Component*...> ECSScene::AddComponents(Entity entity)
{
	return std::make_tuple(AddComponent<Component>(entity)...);
}

template <class Component>
Component* ECSScene::GetComponent(Entity entity)
{
	auto& m = entity_container_.GetComponentMap<Component>();
	auto it = m.find(entity);
	assert(it != m.end());
	return &(it->second);
}

template <class... Component>
std::tuple<Component*...> ECSScene::GetComponents(Entity entity)
{
	return std::make_tuple(GetComponent<Component>(entity)...);
}

template <class Component>
Component* ECSScene::RawGetComponent(Entity entity)
{
	auto& m = entity_container_.GetComponentMap<Component>();
	auto it = m.find(entity);
	if (it == m.end())
	{
		return nullptr;
	}
	return &(it->second);
}

template <class... Component>
std::tuple<Component*...> ECSScene::RawGetComponents(Entity entity)
{
	return std::make_tuple(RawGetComponent<Component>(entity)...);
}

template <class Component>
void ECSScene::DeleteChildEntitiesOwnedByComponent(Entity entity)
{
	Children* children = RawGetComponent<Children>(entity);
	if (children == nullptr)
	{
		return;
	}
	auto& it = children->entities_owned_by_component.find(typeid(Component));
	if (it == children->entities_owned_by_component.end())
	{
		return;
	}
	for (Entity owned_entity : it->second)
	{
		DeleteEntity(owned_entity);
	}
	children->entities_owned_by_component.erase(it);
}

template <class Component>
bool ECSScene::RemoveComponent(Entity entity)
{
	DeleteChildEntitiesOwnedByComponent<Component>(entity);
	return entity_container_.GetComponentMap<Component>().erase(entity) > 0;
}

template <class... Component>
void ECSScene::RemoveComponents(Entity entity)
{
	auto results = (RemoveComponent<Component>(entity), ...);
}

template <class... Component>
std::tuple<Entity, Component*...> ECSScene::CreateEntityWith()
{
	Entity entity = CreateEntity();
	return std::tuple_cat(std::make_tuple(entity), AddComponents<Component...>(entity));
}

template <class OtherComponent, class... Component>
static bool IdIntersection(Entity component_idx, std::map<Entity, OtherComponent>& component_map, std::vector<std::tuple<Entity, Component*...>>& matching_entities)
{
	if (component_map.size() == 0)
	{
		matching_entities.clear();
		return false;
	}
	if (component_idx == 0)
	{
		for (auto& [entity, component_value] : component_map)
		{
			std::tuple<Entity, Component*...> x = {};
			std::get<Entity>(x) = entity;
			std::get<OtherComponent*>(x) = &component_value;
			matching_entities.push_back(x);
		}
	}
	else
	{
		for (auto it = matching_entities.begin(); it != matching_entities.end();)
		{
			auto& map_it = component_map.find(std::get<Entity>(*it));
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

template <class Component>
std::vector<Entity> ECSScene::GetEntitiesWithComponent()
{
	std::vector<Entity> entities;
	for (auto [entity, _] : entity_container_.GetComponentMap<Component>())
	{
		entities.push_back(entity);
	}
	return entities;
}

template <class... Component>
std::vector<std::tuple<Entity, Component*...>> ECSScene::GetEntitiesWith()
{
	int component_idx = 0;
	std::vector<std::tuple<Entity, Component*...>> matching_entities = {};
	(IdIntersection<Component, Component...>(component_idx++, entity_container_.GetComponentMap<Component>(), matching_entities) && ...);
	return matching_entities;
}

template <class... Component>
void ECSScene::DeleteEntitiesWith()
{
	auto entities = GetEntitiesWith<Component...>();
	for (auto tup : entities)
	{
		Entity entity = std::get<0>(tup);
		DeleteEntity(entity);
	}
}

template <class Component>
void ECSScene::ClearComponent()
{
	for (auto [entity, _] : entity_container_.GetComponentMap<Component>())
	{
		DeleteChildEntitiesOwnedByComponent<Component>(entity);
	}
	entity_container_.GetComponentMap<Component>().clear();
}