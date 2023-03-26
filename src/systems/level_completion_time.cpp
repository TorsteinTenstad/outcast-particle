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
	LevelCompletionTimer* level_completion_timer = level.GetSingleton<LevelCompletionTimer>();
	CoinCounter* coin_counter = level.GetSingleton<CoinCounter>();

	LevelState level_state = level.ComputeState();
	if (level.GetMode() == PLAY_MODE && level_state == PLAYING)
	{
		level_completion_timer->duration += dt;
		return;
	}
	if (level_state != COMPLETED)
	{
		return;
	}
	float& current_record = (*level_completion_time_records_)[coin_counter->coin_counter][active_level_id_];
	if (current_record <= 0 || current_record > level_completion_timer->duration)
	{
		current_record = level_completion_timer->duration;
		return;
	}
}
