#include "_pure_DO_systems.hpp"
#include "globals.hpp"
#include "level.hpp"

void ScheduledDeleteSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, scheduled_delete] : level.GetEntitiesWith<ScheduledDelete>())
		if (scheduled_delete->delete_at > 0 && globals.time >= scheduled_delete->delete_at)
		{
			level.DeleteEntity(entity_id);
		}
}