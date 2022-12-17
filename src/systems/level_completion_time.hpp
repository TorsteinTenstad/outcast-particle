#pragma once
#include "game_system.hpp"
#include "level.hpp"
#include "level_completion_time.hpp"
#include "modes.hpp"

class LevelCompletionTimeSystem
{
private:
	std::map<int, float>* level_completion_time_records_;

public:
	LevelCompletionTimeSystem()
	{}
	LevelCompletionTimeSystem(std::map<int, float>* level_completion_time_records)
	{
		level_completion_time_records_ = level_completion_time_records;
	}
	void Update(Mode mode, CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& level_completion_timer_map = level.GetComponent<LevelCompletionTimer>();
		assert(!(level_completion_timer_map.size() > 1));

		if (level_completion_timer_map.size() == 0)
		{
			int entity_id = level.CreateEntityId();
			level_completion_timer_map[entity_id].duration = 0;
		}

		float& duration = level_completion_timer_map.begin()->second.duration;
		if (mode == PLAY_MODE)
		{
			duration += dt;
			return;
		}
		if (mode == LEVEL_COMPLETED_MODE && ((*level_completion_time_records_)[level.id] <= 0 || (*level_completion_time_records_)[level.id] > duration))
		{
			(*level_completion_time_records_)[level.id] = duration;
			return;
		}
	}
};