#pragma once
#include "ecs/component_container_operations.hpp"
#include "ecs/ecs_scene.hpp"

class ECSSceneWithInactiveLayer : public ECSScene
{

public:
	ECSScene inactive_entities;

private:
	template <class... Components>
	void RecursiveMoveEntityActiveState(int entity_id,
		std::map<std::type_index, std::variant<std::map<int, Components>...>>& from_container,
		std::map<std::type_index, std::variant<std::map<int, Components>...>>& to_container)
	{
		if (Children* children = RawGetComponent<Children>(entity_id))
		{
			for (auto& [component_type_id, child_ids] : children->ids_owned_by_component)
			{
				for (auto& child_id : child_ids)
				{
					RecursiveMoveEntityActiveState(child_id, from_container, to_container);
				}
			}
		}
		MoveAllEntriesAtKey(entity_id, from_container, to_container);
	}

public:
	void ActivateEntity(int entity_id)
	{
		RecursiveMoveEntityActiveState(entity_id, inactive_entities.components_, components_);
	}
	void DeactivateEntity(int entity_id)
	{
		RecursiveMoveEntityActiveState(entity_id, components_, inactive_entities.components_);
	}
};