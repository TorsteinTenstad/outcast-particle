#pragma once

#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

#define MINIMUM_PLAYER_VELOCITY_REQUIRED_FOR_FACE_MOVEMENT 10

class VelocityDependentDrawLayerSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& children_map = level.GetComponent<Children>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& position_map = level.GetComponent<Position>();
		auto& radius_map = level.GetComponent<Radius>();

		for (auto& [entity_id, velocity_dependent_draw_layer, draw_info, draw_priority, radius, velocity, position] : level.GetEntitiesWith<VelocityDependentDrawLayer, DrawInfo, DrawPriority, Radius, Velocity, Position>())
		{
			std::vector<int>& children = children_map[entity_id].id_owned_by_component[typeid(VelocityDependentDrawLayer)];

			if (children.size() == 0)
			{
				int id = level.CreateEntityId();
				draw_info_map[id] = *draw_info;
				position_map[id] = *position;
				draw_priority_map[id] = *draw_priority;
				radius_map[id] = *radius;

				draw_priority_map[id].draw_priority++;

				children.push_back(id);
			}

			int child_id = children[0];

			position_map[child_id].position += velocity->velocity * dt;
			sf::Vector2f offset = position_map[child_id].position - position->position;
			sf::Vector2f target_offset = sf::Vector2f(0, 0);
			if (Magnitude(velocity->velocity) > MINIMUM_PLAYER_VELOCITY_REQUIRED_FOR_FACE_MOVEMENT)
			{
				target_offset = 20.f * Normalized(velocity->velocity);
			}
			float snap = 0.01f * dt * 600;
			sf::Vector2f next_offset = (1 - snap) * offset + snap * target_offset;
			position_map[child_id].position = position->position + next_offset;
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};