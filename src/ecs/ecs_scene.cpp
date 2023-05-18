#include "ecs/ecs_scene.hpp"
#include "game.hpp"
#include "utils/random.hpp"
#include "utils/string_parsing.hpp"
#include <cstdlib>
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

Entity ECSScene::CreateEntity()
{
	Entity new_entity(Rand());

	for (auto entity_creation_observer : entity_creation_observers)
	{
		entity_creation_observer->Notify(new_entity);
	}
	return new_entity;
}

bool ECSScene::IdExists(Entity entity)
{
	return entity_container_.Exists(entity);
}

void ECSScene::Clear()
{
	return entity_container_.Clear();
}

Entity ECSScene::CopyEntity(Entity from_entity)
{
	Entity to_entity = CreateEntity();
	entity_container_.CopyEntity(from_entity, to_entity);
	if (Children* children = RawGetComponent<Children>(to_entity))
	{
		children->entities_owned_by_component.clear();
	}
	return to_entity;
}

void ECSScene::DeleteEntity(std::optional<Entity> entity)
{
	if (entity.has_value())
	{
		DeleteEntity(entity.value());
	}
}

void ECSScene::DeleteEntity(Entity entity)
{
	if (Children* children = RawGetComponent<Children>(entity))
	{
		for (auto& [component_type_id, child_entities] : children->entities_owned_by_component)
		{
			for (auto& child : child_entities)
			{
				DeleteEntity(child);
			}
		}
	}
	entity_container_.DeleteEntity(entity);
}
