#include "ecs/ecs_scene.hpp"
#include <functional>
#include <optional>
#include <tuple>

template <class Component>
std::tuple<Entity, Component*> GetSingletonIncludeID(std::function<Entity(ECSScene&)> creation_func)
{
	auto& component_map = entity_container_.GetComponentMap<Component>();

	if (component_map.size() == 0)
	{
		Entity entity = creation_func(*this);
		EnsureExistenceOfComponent<Component>(entity);
	}
	assert(component_map.size() == 1);
	return { component_map.begin()->first, &component_map.begin()->second };
}

template <class Component>
Entity GetSingletonId(std::function<Entity(ECSScene&)> creation_func)
{
	return std::get<Entity>(GetSingletonIncludeID<Component>(creation_func));
}

template <class Component>
Component* GetSingleton(std::function<Entity(ECSScene&)> creation_func)
{
	return std::get<Component*>(GetSingletonIncludeID<Component>(creation_func));
}

template <class Component>
Entity GetSingletonId()
{
	return GetSingletonId<Component>([](ECSScene& scene) { return scene.CreateEntity(); });
}

template <class Component>
Component* GetSingleton()
{
	return GetSingleton<Component>([](ECSScene& scene) { return scene.CreateEntity(); });
}

template <class Component>
std::optional<std::tuple<Entity, Component*>> FindSingletonIncludeID()
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
std::optional<Entity> FindSingletonId()
{
	std::optional<std::tuple<Entity, Component*>> tup = FindSingletonIncludeID<Component>();
	if (!tup.has_value())
	{
		return std::nullopt;
	}
	return std::get<Entity>(tup.value());
}

template <class Component>
Component* FindSingleton()
{
	std::optional<std::tuple<Entity, Component*>> tup = FindSingletonIncludeID<Component>();
	if (!tup.has_value())
	{
		return nullptr;
	}
	return std::get<Component*>(tup.value());
}