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

void ECSSceneWithInactiveLayer::ActivateEntity(int entity_id)
{
	RecursiveMoveEntityActiveState(entity_id, inactive_entities, inactive_entities.components_, components_);
}
void ECSSceneWithInactiveLayer::DeactivateEntity(int entity_id)
{
	RecursiveMoveEntityActiveState(entity_id, *this, components_, inactive_entities.components_);
}
