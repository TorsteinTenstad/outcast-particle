#pragma once
#include "ecs/component_container_operations.hpp"
#include "ecs/ecs_scene.hpp"

class ECSSceneWithInactiveLayer : public ECSScene
{

public:
	ECSScene inactive_entities;

private:
	template <class... Components>
	void RecursiveMoveEntityActiveState(Entity entity, ECSScene& from_scene,
		EntityContainer& from_container,
		EntityContainer& to_container);

public:
	void ActivateEntities(Entity entity);
	void DeactivateEntities(Entity entity);
	void ActivateEntities(std::vector<Entity> entities);
	void DeactivateEntities(std::vector<Entity> entities);

	template <class Component>
	void ActivateComponent(Entity entity);

	template <class Component>
	void DeactivateComponent(Entity entity);
};

#include "ecs/ecs_scene_with_inactive_layer.tpp"