#pragma once
#include "components.hpp"
#include "entity_creation_observer.hpp"
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
	static int next_available_entity_id_;
	std::map<std::type_index, ComponentMap> components_;

protected:
	std::vector<EntityCreationObserver*> entity_creation_observers;

public:
	void AddEntityCreationObserver(EntityCreationObserver* entity_creation_observer);
	void RemoveEntityCreationObserver(EntityCreationObserver* entity_creation_observer);

protected:
	template <class Component>
	std::map<int, Component>& GetComponentMap();

protected:
	template <class Component>
	bool HasComponent(int entity_id);

public:
	template <class... Component>
	bool HasComponents(int entity_id);

	int CreateEntityId();

	template <class Component>
	Component* EnsureExistenceOfComponent(int entity_id);

	template <class... Component>
	std::tuple<Component*...> EnsureExistenceOfComponents(int entity_id);

	template <class Component>
	Component* AddComponent(int entity_id);

	template <class Component>
	Component* AddComponent(int entity_id, Component&& value);

	template <class... Component>
	std::tuple<Component*...> AddComponents(int entity_id);

	template <class Component>
	Component* GetComponent(int entity_id);

	template <class... Component>
	std::tuple<Component*...> GetComponents(int entity_id);

	template <class Component>
	Component* RawGetComponent(int entity_id);

	template <class... Component>
	std::tuple<Component*...> RawGetComponents(int entity_id);

private:
	template <class Component>
	void DeleteChildEntitiesOwnedByComponent(int entity_id);

protected:
	template <class Component>
	bool RemoveComponent(int entity_id);

public:
	template <class... Component>
	bool RemoveComponents(int entity_id);

	template <class... Component>
	std::vector<std::tuple<int, Component*...>> GetEntitiesWith();

	template <class... Component>
	std::tuple<int, Component*...> CreateEntityWith();

	template <class... Component>
	void DeleteEntitiesWith();

	template <class Component>
	void ClearComponent();

	void DeleteEntity(int id);

	void DeleteEntity(std::optional<int> id);

	int CopyEntity(int from_id);

	template <class Component>
	std::tuple<int, Component*> GetSingletonIncludeID(std::function<int(ECSScene&)> creation_func);

	template <class Component>
	Component* GetSingleton(std::function<int(ECSScene&)> creation_func);

	template <class Component>
	std::tuple<int, Component*> GetSingletonIncludeID();

	template <class Component>
	Component* GetSingleton();
};

#include "ecs_scene.tpp"