#pragma once
#include "level_completion_time.hpp"
#include "components/coin.hpp"
#include "components/player_input_tracker.hpp"
#include "components/timers.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "level_mode.hpp"

class ScoreHasBeenUpdated
{};

void LevelCompletionTimeSystem::SetLevelCompletionTimeRecords(RecordsManager* records)
{
	records_ = records;
}
void LevelCompletionTimeSystem::Update(Level& level, float dt)
{
	LevelCompletionTimer* level_completion_timer = level.GetSingleton<LevelCompletionTimer>();
	CoinCounter* coin_counter = level.GetSingleton<CoinCounter>();
	PlayerInputTracker* player_input_tracker = level.GetSingleton<PlayerInputTracker>();

	LevelState level_state = level.ComputeState();
	if (level.GetMode() == PLAY_MODE && level_state == PLAYING)
	{
		level_completion_timer->duration += dt;
		return;
	}
	if (level_state != COMPLETED || level.GetEntitiesWithComponent<ScoreHasBeenUpdated>().size() > 0) { return; }
	records_->UpdateRecord(active_level_id_, coin_counter->coin_counter, player_input_tracker->neutral_was_used, level_completion_timer->duration);
	level.CreateEntityWith<ScoreHasBeenUpdated>();
}
