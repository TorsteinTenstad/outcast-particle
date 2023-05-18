#pragma once
#include "ecs/ecs_scene_with_inactive_layer.hpp"
#include "ecs/component_container_operations.hpp"

void ECSSceneWithInactiveLayer::ActivateEntities(Entity entity)
{
	RecursiveMoveEntityActiveState(entity, inactive_entities, inactive_entities.entity_container_, entity_container_);
}
void ECSSceneWithInactiveLayer::DeactivateEntities(Entity entity)
{
	RecursiveMoveEntityActiveState(entity, *this, entity_container_, inactive_entities.entity_container_);
}
void ECSSceneWithInactiveLayer::ActivateEntities(std::vector<Entity> entities)
{
	for (auto entity : entities)
	{
		ActivateEntities(entity);
	}
}
void ECSSceneWithInactiveLayer::DeactivateEntities(std::vector<Entity> entities)
{
	for (auto entity : entities)
	{
		DeactivateEntities(entity);
	}
}
