#include "ecs/ecs_scene.hpp"
#include "game.hpp"
#include "utils/string_parsing.hpp"
#include <functional>
#include <iostream>
#include <optional>
#include <string>

void ECSScene::AddEntityCreationObserver(EntityCreationObserver* entity_creation_observer)
{
	entity_creation_observers.push_back(entity_creation_observer);
}

void ECSScene::RemoveEntityCreationObserver(EntityCreationObserver* entity_creation_observer)
{
	auto it = std::find(entity_creation_observers.begin(), entity_creation_observers.end(), entity_creation_observer);
	if (it != entity_creation_observers.end())
	{
		entity_creation_observers.erase(it);
	}
}

int ECSScene::CreateEntityId()
{
	int new_id = next_available_entity_id_++;
	for (auto entity_creation_observer : entity_creation_observers)
	{
		entity_creation_observer->Notify(new_id);
	}
	return new_id;
}

bool ECSScene::IdExists(int entity_id)
{
	return entity_container_.Exists(entity_id);
}

void ECSScene::Clear()
{
	return entity_container_.Clear();
}

int ECSScene::CopyEntity(int from_id)
{
	int to_id = CreateEntityId();
	entity_container_.CopyEntity(from_id, to_id);
	if (Children* children = RawGetComponent<Children>(to_id))
	{
		children->ids_owned_by_component.clear();
	}
	return to_id;
}

void ECSScene::DeleteEntity(std::optional<int> id)
{
	if (id.has_value())
	{
		DeleteEntity(id.value());
	}
}

void ECSScene::DeleteEntity(int id)
{
	if (Children* children = RawGetComponent<Children>(id))
	{
		for (auto& [component_type_id, child_ids] : children->ids_owned_by_component)
		{
			for (auto& child_id : child_ids)
			{
				DeleteEntity(child_id);
			}
		}
	}
	entity_container_.DeleteEntity(id);
}
