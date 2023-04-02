#include "game_system.hpp"
#include <algorithm>
#include <functional>
#include <string>

class PauseMode : public GameSystem
{
private:
	std::function<void(std::string)> set_level_;
	const std::map<std::string, std::vector<std::string>>* level_groups_;
	const std::map<int, std::map<std::string, float>>* level_completion_time_records_;

	void SetupPauseMenu(Level& level, LevelMode previous_mode);

public:
	using GameSystem::GameSystem;
	void Give(
		std::function<void(std::string)> set_level,
		const std::map<std::string, std::vector<std::string>>* level_groups,
		const std::map<int, std::map<std::string, float>>* level_completion_time_records);
	void Update(Level& level, float dt);
};