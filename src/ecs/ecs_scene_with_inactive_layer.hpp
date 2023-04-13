#pragma once
#include "ecs/component_container_operations.hpp"
#include "ecs/ecs_scene.hpp"

class ECSSceneWithInactiveLayer : public ECSScene
{

public:
	ECSScene inactive_entities;

private:
	template <class... Components>
	void RecursiveMoveEntityActiveState(int entity_id, ECSScene& from_scene,
		std::map<std::type_index, std::variant<std::map<int, Components>...>>& from_container,
		std::map<std::type_index, std::variant<std::map<int, Components>...>>& to_container);

public:
	int CreateEntityId() override;

	void ActivateEntity(int entity_id);
	void DeactivateEntity(int entity_id);

	template <class Component>
	void ActivateComponent(int entity_id);

	template <class Component>
	void DeactivateComponent(int entity_id);
};

#include "ecs/ecs_scene_with_inactive_layer.tpp"