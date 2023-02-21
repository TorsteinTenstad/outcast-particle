#pragma once
#include "game_system.hpp"
#include "level.hpp"
#include "level_completion_time.hpp"
#include "level_mode.hpp"

class LevelCompletionTimeSystem : public GameSystem
{
private:
	std::map<int, std::map<std::string, float>>* level_completion_time_records_;

public:
	using GameSystem::GameSystem;
	void SetLevelCompletionTimeRecords(std::map<int, std::map<std::string, float>>* level_completion_time_records);
	void Update(Level& level, float dt);
};