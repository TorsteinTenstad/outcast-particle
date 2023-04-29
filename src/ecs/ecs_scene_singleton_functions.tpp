#include "ecs/ecs_scene.hpp"
#include <functional>
#include <optional>
#include <tuple>

template <class Component>
std::tuple<int, Component*> GetSingletonIncludeID(std::function<int(ECSScene&)> creation_func)
{
	auto& component_map = entity_container_.GetComponentMap<Component>();

	if (component_map.size() == 0)
	{
		int entity_id = creation_func(*this);
		EnsureExistenceOfComponent<Component>(entity_id);
	}
	assert(component_map.size() == 1);
	return { component_map.begin()->first, &component_map.begin()->second };
}

template <class Component>
int GetSingletonId(std::function<int(ECSScene&)> creation_func)
{
	return std::get<int>(GetSingletonIncludeID<Component>(creation_func));
}

template <class Component>
Component* GetSingleton(std::function<int(ECSScene&)> creation_func)
{
	return std::get<Component*>(GetSingletonIncludeID<Component>(creation_func));
}

template <class Component>
int GetSingletonId()
{
	return GetSingletonId<Component>([](ECSScene& scene) { return scene.CreateEntityId(); });
}

template <class Component>
Component* GetSingleton()
{
	return GetSingleton<Component>([](ECSScene& scene) { return scene.CreateEntityId(); });
}

template <class Component>
std::optional<std::tuple<int, Component*>> FindSingletonIncludeID()
{
	auto& component_map = entity_container_.GetComponentMap<Component>();

	if (component_map.size() == 0)
	{
		return std::nullopt;
	}
	assert(component_map.size() == 1);
	return std::make_tuple(component_map.begin()->first, &component_map.begin()->second);
}

template <class Component>
std::optional<int> FindSingletonId()
{
	std::optional<std::tuple<int, Component*>> tup = FindSingletonIncludeID<Component>();
	if (!tup.has_value())
	{
		return std::nullopt;
	}
	return std::get<int>(tup.value());
}

template <class Component>
Component* FindSingleton()
{
	std::optional<std::tuple<int, Component*>> tup = FindSingletonIncludeID<Component>();
	if (!tup.has_value())
	{
		return nullptr;
	}
	return std::get<Component*>(tup.value());
}