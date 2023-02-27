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
	LevelCompletionTimer* level_completion_timer = GetSingleton<LevelCompletionTimer>(level);
	CoinCounter* coin_counter = GetSingleton<CoinCounter>(level);

	if (level.GetMode() == PLAY_MODE)
	{
		level_completion_timer->duration += dt;
		return;
	}
	if (level.ComputeState() == COMPLETED && ((*level_completion_time_records_)[coin_counter->coin_counter][active_level_id_] <= 0 || (*level_completion_time_records_)[coin_counter->coin_counter][active_level_id_] > level_completion_timer->duration))
	{
		(*level_completion_time_records_)[coin_counter->coin_counter][active_level_id_] = level_completion_timer->duration;
		return;
	}
}
