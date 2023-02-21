
#include "_pure_DO_systems.hpp"
#include "components/goal.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

void GoalSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != PLAY_MODE)
	{
		return;
	}
	for (auto& [entity_id, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
	{
		for (auto intersecting_id : intersection->intersecting_ids)
		{
			if (!level.HasComponents<Goal, SoundInfo>(intersecting_id))
			{
				continue;
			}
			level.RemoveComponents<Intersection>(entity_id);
			level.RemoveComponents<Velocity>(entity_id);
			level.RemoveComponents<Player>(entity_id);
			level.GetComponent<Goal>(intersecting_id)->is_goal = true;
			level.GetComponent<SoundInfo>(intersecting_id)->play_sound = true;
			break;
		}
	}
}
