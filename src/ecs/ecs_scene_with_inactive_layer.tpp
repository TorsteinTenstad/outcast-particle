#include "ecs/ecs_scene_with_inactive_layer.hpp"
#include <cassert>

template <class... Components>
void ECSSceneWithInactiveLayer::RecursiveMoveEntityActiveState(Entity entity, ECSScene& from_scene,
	EntityContainer& from_container,
	EntityContainer& to_container)
{
	if (Children* children = from_scene.RawGetComponent<Children>(entity))
	{
		for (auto& [component_type_id, child_entities] : children->entities_owned_by_component)
		{
			for (auto& child : child_entities)
			{
				RecursiveMoveEntityActiveState(child, from_scene, from_container, to_container);
			}
		}
	}
	from_container.CopyEntity(entity, to_container);
	from_container.DeleteEntity(entity);
}

template <class Component>
void ECSSceneWithInactiveLayer::ActivateComponent(Entity entity)
{
	assert(!inactive_entities.HasComponents<Children>(entity)); //Activating/Deactivating components on entities that have children is not supported as of now.
	assert(inactive_entities.HasComponents<Component>(entity));
	std::type_index type_index = typeid(Component);
	MoveComponentAtKey(entity, type_index, inactive_entities.components_.at(type_index), components_);
}

template <class Component>
void ECSSceneWithInactiveLayer::DeactivateComponent(Entity entity)
{
	assert(!HasComponents<Children>(entity)); //Activating/Deactivating components on entities that have children is not supported as of now.
	assert(HasComponents<Component>(entity));
	std::type_index type_index = typeid(Component);
	MoveComponentAtKey(entity, type_index, components_.at(type_index), inactive_entities.components_);
}