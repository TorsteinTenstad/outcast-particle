#pragma once
#include "game_system.hpp"
#include "level.hpp"
#include "level_completion_time.hpp"
#include "level_mode.hpp"
#include "records_manager.hpp"

class LevelCompletionTimeSystem : public GameSystem
{
private:
	RecordsManager* records_;

public:
	using GameSystem::GameSystem;
	void SetLevelCompletionTimeRecords(RecordsManager* records);
	void Update(Level& level, float dt);
};