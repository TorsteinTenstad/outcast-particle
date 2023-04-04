#include "ecs/ecs_scene.hpp"
#include "game.hpp"
#include "utils/string_parsing.hpp"
#include <functional>
#include <iostream>
#include <optional>
#include <string>

int ECSScene::next_available_entity_id_ = 0;

void ECSScene::AddEntityCreationObserver(EntityCreationObserver* entity_creation_observer)
{
	entity_creation_observers.push_back(entity_creation_observer);
}

void ECSScene::RemoveEntityCreationObserver(EntityCreationObserver* entity_creation_observer)
{
	auto it = std::find(entity_creation_observers.begin(), entity_creation_observers.end(), entity_creation_observer);
	assert(it != entity_creation_observers.end());
	entity_creation_observers.erase(it);
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

int ECSScene::CopyEntity(int from_id)
{
	int to_id = CreateEntityId();
	for (auto& [component_type_id, component_map_variant] : components_)
	{
		std::visit([from_id, to_id, component_type_id = component_type_id](auto& component_map) {
			if (component_map.count(from_id) == 0)
			{
				return;
			}
			if (component_type_id == typeid(Children)) // We do not copy children entities, but an empty children component gets added
			{
				component_map[to_id];
				return;
			}
			component_map.emplace(to_id, component_map.at(from_id));
		},
			component_map_variant);
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
	for (auto& [_, component_map_variant] : components_)
	{
		std::visit([id](auto& component_map) {
			if (component_map.count(id) != 0)
			{
				component_map.erase(id);
			} },
			component_map_variant);
	}
}
