#include "_pure_DO_systems.hpp"
#include "globals.hpp"
#include "level.hpp"

void ScheduledDeleteSystem::Update(Level& level, float dt)
{
	auto& scheduled_delete_map = level.GetComponent<ScheduledDelete>();
	for (auto it = scheduled_delete_map.cbegin(), next_it = it; it != scheduled_delete_map.cend(); it = next_it)
	{
		++next_it;
		auto entity_id = it->first;
		auto scheduled_delete = it->second;
		if (scheduled_delete.delete_at > 0 && globals.time >= scheduled_delete.delete_at)
		{
			level.DeleteEntity(entity_id);
		}
	}
}