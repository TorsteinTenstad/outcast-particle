#include "components/scheduled_delete.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"

void ScheduledDeleteSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, scheduled_delete] : level.GetEntitiesWith<ScheduledDelete>())
	{
		if (scheduled_delete->delete_at.has_value() && globals.time >= scheduled_delete->delete_at.value())
		{
			level.DeleteEntity(entity_id);
		}
		if (scheduled_delete->frames_left_to_live.has_value())
		{
			int frames_left_to_live = scheduled_delete->frames_left_to_live.value();
			if (frames_left_to_live == 0)
			{
				level.DeleteEntity(entity_id);
			}
			else
			{
				scheduled_delete->frames_left_to_live = frames_left_to_live - 1;
			}
		}
	}
}