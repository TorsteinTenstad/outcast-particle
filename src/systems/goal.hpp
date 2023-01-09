
#include "components/goal.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <iostream>
class GoalSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& goal_map = level.GetComponent<Goal>();
		auto& sound_info_map = level.GetComponent<SoundInfo>();
		for (auto& [entity_id, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
		{
			for (auto intersection_id : intersection->intersecting_ids)
			{
				if (goal_map.count(intersection_id) > 0)
				{
					level.GetComponent<Intersection>().erase(entity_id);
					level.GetComponent<Velocity>().erase(entity_id);
					level.GetComponent<Player>().erase(entity_id);
					goal_map[intersection_id].is_goal = true;
					sound_info_map[intersection_id].play_sound = true;
				}
			}
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};
