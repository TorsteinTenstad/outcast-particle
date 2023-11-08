
#include "components/goal.hpp"
#include "components/collision.hpp"
#include "components/draw_info.hpp"
#include "components/intersection.hpp"
#include "components/player.hpp"
#include "components/scheduled_delete.hpp"
#include "components/screen_wide_shader_effects.hpp"
#include "components/size.hpp"
#include "components/sound_info.hpp"
#include "components/trail.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include "utils/physics.hpp"
#include <cmath>
#include <iostream>

void GoalSystem::Update(Level& level, float dt)
{
	if (level.GetMode() == EDIT_MODE) { return; }
	for (auto& [entity, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
	{
		for (auto intersecting_id : intersection->entities_entered_this_frame)
		{
			if (!level.HasComponents<Goal>(intersecting_id))
			{
				continue;
			}
			if (SoundInfo* sound_info = level.RawGetComponent<SoundInfo>(intersecting_id))
			{
				sound_info->play_sound.push(DEFAULT);
			}
			else
			{
				assert(false);
			}
			level.RemoveComponents<Player>(entity);
			level.AddComponent<PlayerInGoalAnimation>(entity);
			level.RemoveComponents<Collision>(entity);
			level.RemoveComponents<ForceVisualization>(entity);
			level.AddComponent<ScheduledDelete>(entity)->delete_at = globals.time + 2;
			level.GetComponent<Goal>(intersecting_id)->is_goal = true;
			break;
		}
	}
}
