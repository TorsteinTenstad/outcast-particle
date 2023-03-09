#include "ecs_scene.hpp"
#include "game.hpp"
#include "string_parsing_utils.hpp"
#include <functional>
#include <iostream>
#include <optional>
#include <string>

int ESCScene::next_available_entity_id_ = 0;

int ESCScene::CreateEntityId()
{
	return next_available_entity_id_++;
}

int ESCScene::CopyEntity(int from_id)
{
	int to_id = CreateEntityId();
	for (auto& [component_type_id, component_map_variant] : components_)
	{
		if (component_type_id == typeid(Children))
		{
			continue;
		}
		std::visit([from_id, to_id](auto& component_map) {
			if (component_map.count(from_id) != 0)
			{
				component_map[to_id] = component_map[from_id];
			} },
			component_map_variant);
	}
	return to_id;
}

void ESCScene::DeleteEntity(std::optional<int> id)
{
	if (id.has_value())
	{
		DeleteEntity(id.value());
	}
}

void ESCScene::DeleteEntity(int id)
{
	if (HasComponents<Children>(id))
	{
		for (auto& [component_type_id, child_ids] : GetComponent<Children>(id)->ids_owned_by_component)
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
