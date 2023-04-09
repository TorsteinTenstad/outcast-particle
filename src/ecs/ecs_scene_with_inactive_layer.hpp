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
		MoveAllComponentsAtKey(entity_id, from_container, to_container);
	}

public:
	int CreateEntityId() override
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

	void
	ActivateEntity(int entity_id)
	{
		RecursiveMoveEntityActiveState(entity_id, inactive_entities.components_, components_);
	}
	void DeactivateEntity(int entity_id)
	{
		RecursiveMoveEntityActiveState(entity_id, components_, inactive_entities.components_);
	}

	template <class Component>
	void ActivateComponent(int entity_id)
	{
		assert(!inactive_entities.HasComponents<Children>(entity_id)); //Activating/Deactivating components on entities that have children is not supported as of now.
		assert(inactive_entities.HasComponents<Component>(entity_id));
		std::type_index type_index = typeid(Component);
		MoveComponentAtKey(entity_id, type_index, inactive_entities.components_.at(type_index), components_);
	}

	template <class Component>
	void DeactivateComponent(int entity_id)
	{
		assert(!HasComponents<Children>(entity_id)); //Activating/Deactivating components on entities that have children is not supported as of now.
		assert(HasComponents<Component>(entity_id));
		std::type_index type_index = typeid(Component);
		MoveComponentAtKey(entity_id, type_index, components_.at(type_index), inactive_entities.components_);
	}
};