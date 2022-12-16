#include "PCH.hpp"
#include "components/goal.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

class GoalSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& intersection_map = level.GetComponent<Intersection>();
		auto& player_map = level.GetComponent<Player>();
		auto& goal_map = level.GetComponent<Goal>();
		auto& sound_info_map = level.GetComponent<SoundInfo>();
		for (auto& [entity_id, _] : player_map)
		{
			for (auto& intersection_id : intersection_map[entity_id].intersecting_ids)
			{
				if (goal_map.count(intersection_id) != 0)
				{
					goal_map[intersection_id].is_goal = true;
					sound_info_map[intersection_id].play_sound = true;
				}
			}
		}
	}
};
