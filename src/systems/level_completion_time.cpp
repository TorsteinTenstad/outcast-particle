#pragma once
#include "level_completion_time.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "level_mode.hpp"

void LevelCompletionTimeSystem::SetLevelCompletionTimeRecords(std::map<int, std::map<std::string, float>>* level_completion_time_records)
{
	level_completion_time_records_ = level_completion_time_records;
}
void LevelCompletionTimeSystem::Update(Level& level, float dt)
{
	auto& level_completion_timer_map = level.GetComponent<LevelCompletionTimer>();
	CoinCounter* coin_counter = GetSingleton<CoinCounter>(level);
	assert(!(level_completion_timer_map.size() > 1));

	if (level_completion_timer_map.size() == 0)
	{
		int entity_id = level.CreateEntityId();
		level_completion_timer_map[entity_id].duration = 0;
	}

	float& duration = level_completion_timer_map.begin()->second.duration;
	if (level.GetMode() == PLAY_MODE)
	{
		duration += dt;
		return;
	}
	if (level.ComputeState() == COMPLETED && ((*level_completion_time_records_)[coin_counter->coin_counter][active_level_id_] <= 0 || (*level_completion_time_records_)[coin_counter->coin_counter][active_level_id_] > duration))
	{
		(*level_completion_time_records_)[coin_counter->coin_counter][active_level_id_] = duration;
		return;
	}
}
