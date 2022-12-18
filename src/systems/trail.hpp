#pragma once
#include "game_system.hpp"
#include "level.hpp"

#define TRAIL_N 64

class TrailSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& velocity_map = level.GetComponent<Velocity>();
		auto& trail_map = level.GetComponent<Trail>();
		auto& radius_map = level.GetComponent<Radius>();

		for (auto& [entity_id, trail] : trail_map)
		{
			assert(radius_map.count(entity_id) > 0);

			trail.path.insert(trail.path.begin(), -velocity_map[entity_id].velocity * dt - (radius_map[entity_id].radius / TRAIL_N) * Normalized(velocity_map[entity_id].velocity));
			if (trail.path.size() > TRAIL_N)
			{
				trail.path.pop_back();
			}
			if (trail.path.size() != trail.widths.size())
			{
				trail.widths.clear();
				for (unsigned i = 0; i < trail.path.size(); ++i)
				{
					trail.widths.push_back(0.8 * radius_map[entity_id].radius * pow((float)(trail.path.size() - i) / trail.path.size(), 1.5));
				}
			}
		}
	}
	void OnEnterMode(Mode mode) {};
	void OnExitMode(Mode mode) {};
	void OnEnterLevel(int level_id) {};
	void OnExitLevel(int level_id) {};
};