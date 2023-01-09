#pragma once

#include "components/intersection.hpp"
#include "components/kill_on_intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <iostream>

class KillOnIntersectionSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& intersection_map = level.GetComponent<Intersection>();
		auto& kill_on_intersection_map = level.GetComponent<KillOnIntersection>();
		auto& sound_info_map = level.GetComponent<SoundInfo>();

		for (auto it = intersection_map.cbegin(), next_it = it; it != intersection_map.cend(); it = next_it)
		{
			++next_it;
			auto entity_id = it->first;
			auto intersection = it->second;
			for (auto& i : intersection.intersecting_ids)
			{
				if (kill_on_intersection_map.count(i) != 0)
				{
					level.GetComponent<ScheduledDelete>()[entity_id].delete_at = globals.time + 2;
					sound_info_map[i].play_sound = true;
				}
			}
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};