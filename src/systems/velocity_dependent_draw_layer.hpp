#pragma once
#include "PCH.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

class VelocityDependentDrawLayerSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& velocity_dependent_draw_layer_map = level.GetComponent<VelocityDependentDrawLayer>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& position_map = level.GetComponent<Position>();
		auto& velocity_map = level.GetComponent<Velocity>();
		auto& width_and_height_map = level.GetComponent<WidthAndHeight>();
		auto& radius_map = level.GetComponent<Radius>();
		auto& border_map = level.GetComponent<Border>();

		for (auto it = velocity_dependent_draw_layer_map.begin(), next_it = it; it != velocity_dependent_draw_layer_map.end(); it = next_it)
		{
			++next_it;
			auto& entity_id = it->first;
			auto& velocity_dependent_draw_layer = it->second;
			if (velocity_dependent_draw_layer.owned_entity == -1)
			{
				int id = level.CreateEntityId();
				draw_info_map[id] = draw_info_map[entity_id];
				position_map[id] = position_map[entity_id];
				draw_priority_map[id] = draw_priority_map[entity_id];
				radius_map[id] = radius_map[entity_id];

				draw_priority_map[id].draw_priority++;
				velocity_dependent_draw_layer.owned_entity = id;
				draw_info_map[id].image_path = velocity_dependent_draw_layer.image_path;
			}
			position_map[velocity_dependent_draw_layer.owned_entity].position += velocity_map[entity_id].velocity * dt;
			sf::Vector2f offset = position_map[velocity_dependent_draw_layer.owned_entity].position - position_map[entity_id].position;
			sf::Vector2f target_offset = sf::Vector2f(0, 0);
			if (Magnitude(velocity_map[entity_id].velocity) > 0)
			{
				target_offset = 50.f * Normalized(velocity_map[entity_id].velocity);
			}
			float snap = 0.01f * dt * 600;
			sf::Vector2f next_offset = (1 - snap) * offset + snap * target_offset;
			position_map[velocity_dependent_draw_layer.owned_entity].position = position_map[entity_id].position + next_offset;
		}
	}
	void OnEnterMode(Mode mode) {};
	void OnExitMode(Mode mode) {};
	void OnEnterLevel(int level_id) {};
	void OnExitLevel(int level_id) {};
};