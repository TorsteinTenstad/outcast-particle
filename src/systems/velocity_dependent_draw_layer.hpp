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
		auto& children_map = level.GetComponent<Children>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& position_map = level.GetComponent<Position>();
		auto& velocity_map = level.GetComponent<Velocity>();
		auto& width_and_height_map = level.GetComponent<WidthAndHeight>();
		auto& radius_map = level.GetComponent<Radius>();

		for (auto& [entity_id, velocity_dependent_draw_layer] : velocity_dependent_draw_layer_map)
		{
			std::vector<int>& children = children_map[entity_id].id_owned_by_component[typeid(VelocityDependentDrawLayer)];

			if (children.size() == 0)
			{
				int id = level.CreateEntityId();
				draw_info_map[id] = draw_info_map[entity_id];
				position_map[id] = position_map[entity_id];
				draw_priority_map[id] = draw_priority_map[entity_id];
				radius_map[id] = radius_map[entity_id];

				draw_priority_map[id].draw_priority++;

				children.push_back(id);
			}

			int child_id = children[0];

			position_map[child_id].position += velocity_map[entity_id].velocity * dt;
			sf::Vector2f offset = position_map[child_id].position - position_map[entity_id].position;
			sf::Vector2f target_offset = sf::Vector2f(0, 0);
			if (Magnitude(velocity_map[entity_id].velocity) > 0)
			{
				target_offset = 20.f * Normalized(velocity_map[entity_id].velocity);
			}
			float snap = 0.01f * dt * 600;
			sf::Vector2f next_offset = (1 - snap) * offset + snap * target_offset;
			position_map[child_id].position = position_map[entity_id].position + next_offset;
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};