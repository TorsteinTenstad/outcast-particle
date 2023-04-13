#include "ecs/ecs_scene_with_inactive_layer.hpp"
#include <cassert>

template <class... Components>
void ECSSceneWithInactiveLayer::RecursiveMoveEntityActiveState(int entity_id, ECSScene& from_scene,
	std::map<std::type_index, std::variant<std::map<int, Components>...>>& from_container,
	std::map<std::type_index, std::variant<std::map<int, Components>...>>& to_container)
{
	if (Children* children = from_scene.RawGetComponent<Children>(entity_id))
	{
		for (auto& [component_type_id, child_ids] : children->ids_owned_by_component)
		{
			for (auto& child_id : child_ids)
			{
				RecursiveMoveEntityActiveState(child_id, from_scene, from_container, to_container);
			}
		}
	}
	MoveAllComponentsAtKey(entity_id, from_container, to_container);
}

template <class Component>
void ECSSceneWithInactiveLayer::ActivateComponent(int entity_id)
{
	assert(!inactive_entities.HasComponents<Children>(entity_id)); //Activating/Deactivating components on entities that have children is not supported as of now.
	assert(inactive_entities.HasComponents<Component>(entity_id));
	std::type_index type_index = typeid(Component);
	MoveComponentAtKey(entity_id, type_index, inactive_entities.components_.at(type_index), components_);
}

template <class Component>
void ECSSceneWithInactiveLayer::DeactivateComponent(int entity_id)
{
	assert(!HasComponents<Children>(entity_id)); //Activating/Deactivating components on entities that have children is not supported as of now.
	assert(HasComponents<Component>(entity_id));
	std::type_index type_index = typeid(Component);
	MoveComponentAtKey(entity_id, type_index, components_.at(type_index), inactive_entities.components_);
}