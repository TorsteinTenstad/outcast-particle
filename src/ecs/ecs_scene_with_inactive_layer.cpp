#pragma once
#include "ecs/ecs_scene_with_inactive_layer.hpp"
#include "ecs/component_container_operations.hpp"

int ECSSceneWithInactiveLayer::CreateEntityId()
{
	int new_id = next_available_entity_id_++;
	while (inactive_entities.IdExists(new_id))
	{
		new_id = next_available_entity_id_++;
	}

	for (auto entity_creation_observer : entity_creation_observers)
	{
		entity_creation_observer->Notify(new_id);
	}
	return new_id;
}

void ECSSceneWithInactiveLayer::ActivateEntities(int entity_id)
{
	RecursiveMoveEntityActiveState(entity_id, inactive_entities, inactive_entities.entity_container_, entity_container_);
}
void ECSSceneWithInactiveLayer::DeactivateEntities(int entity_id)
{
	RecursiveMoveEntityActiveState(entity_id, *this, entity_container_, inactive_entities.entity_container_);
}
void ECSSceneWithInactiveLayer::ActivateEntities(std::vector<int> entities)
{
	for (auto entity_id : entities)
	{
		ActivateEntities(entity_id);
	}
}
void ECSSceneWithInactiveLayer::DeactivateEntities(std::vector<int> entities)
{
	for (auto entity_id : entities)
	{
		DeactivateEntities(entity_id);
	}
}
