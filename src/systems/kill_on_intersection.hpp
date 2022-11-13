#pragma once
#include "PCH.hpp"
#include "components/intersection.hpp"
#include "components/kill_on_intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

class KillOnIntersectionSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, Intersection>& intersection_map = level.GetComponent<Intersection>();
		std::map<int, KillOnIntersection>& kill_on_intersection_map = level.GetComponent<KillOnIntersection>();
		for (auto& [entity_id, intersection] : intersection_map)
		{
			for (auto& i : intersection.intersecting_ids)
			{
				if (kill_on_intersection_map.count(i) != 0)
				{
					level.DeleteEntity(entity_id);
				}
			}
		}
	}
};