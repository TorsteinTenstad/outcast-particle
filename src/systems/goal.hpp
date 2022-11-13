#include "PCH.hpp"
#include "components/goal.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

class GoalSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, Intersection>& intersection_map = level.GetComponent<Intersection>();
		std::map<int, Player>& player_map = level.GetComponent<Player>();
		std::map<int, Goal>& goal_map = level.GetComponent<Goal>();
		for (auto& [entity_id, _] : player_map)
		{
			(void)_;
			for (auto& intersection_id : intersection_map[entity_id].intersecting_ids)
			{
				if (goal_map.count(intersection_id) != 0)
				{
					goal_map[intersection_id].is_goal = true;
					std::cout << "goal"
							  << "\n";
				}
			}
		}
	}
};
