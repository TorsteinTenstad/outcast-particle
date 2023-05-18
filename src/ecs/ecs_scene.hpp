#pragma once
#include "components/children.hpp"
#include "ecs/entity.hpp"
#include "ecs/entity_creation_observer.hpp"
#include "entity_container.hpp"
#include <functional>
#include <map>
#include <optional>
#include <typeindex>
#include <variant>
#include <vector>

class EntityCreationObserver;

class ECSScene
{
	friend class ECSSceneWithInactiveLayer;

protected:
	EntityContainer entity_container_;

protected:
	std::vector<EntityCreationObserver*> entity_creation_observers;

public:
	void AddEntityCreationObserver(EntityCreationObserver* entity_creation_observer);
	void RemoveEntityCreationObserver(EntityCreationObserver* entity_creation_observer);

protected:
	template <class Component>
	bool HasComponent(Entity entity);

public:
	template <class... Component>
	bool HasComponents(Entity entity);

	bool IdExists(Entity entity);

	Entity CreateEntity();
	void Clear();

	template <class Component>
	Component* EnsureExistenceOfComponent(Entity entity);

	template <class... Component>
	std::tuple<Component*...> EnsureExistenceOfComponents(Entity entity);

	template <class Component>
	Component* AddComponent(Entity entity);

	template <class Component>
	Component* AddComponent(Entity entity, Component&& value);

	template <class... Component>
	std::tuple<Component*...> AddComponents(Entity entity);

	template <class Component>
	Component* GetComponent(Entity entity);

	template <class... Component>
	std::tuple<Component*...> GetComponents(Entity entity);

	template <class Component>
	Component* RawGetComponent(Entity entity);

	template <class... Component>
	std::tuple<Component*...> RawGetComponents(Entity entity);

	template <class Component>
	void DeleteChildEntitiesOwnedByComponent(Entity entity);

protected:
	template <class Component>
	bool RemoveComponent(Entity entity);

public:
	template <class... Component>
	bool RemoveComponents(Entity entity);

	template <class Component>
	std::vector<Entity> GetEntitiesWithComponent();

	template <class... Component>
	std::vector<std::tuple<Entity, Component*...>> GetEntitiesWith();

	template <class... Component>
	std::tuple<Entity, Component*...> CreateEntityWith();

	template <class... Component>
	void DeleteEntitiesWith();

	template <class Component>
	void ClearComponent();

	void DeleteEntity(Entity entity);

	void DeleteEntity(std::optional<Entity> entity);

	Entity CopyEntity(Entity from_id);

#include "ecs/ecs_scene_singleton_functions.tpp"
};

#include "ecs_scene.tpp"