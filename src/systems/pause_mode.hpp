#include "game_system.hpp"
#include <algorithm>
#include <functional>
#include <string>

class PauseMode : public GameSystem
{
private:
	std::function<void(std::string)> set_level_;
	std::function<void(bool)> set_edit_mode_;
	const std::map<std::string, std::vector<std::string>>* level_groups_;
	const std::map<int, std::map<std::string, float>>* level_completion_time_records_;

	void SetupPauseMenu(Level& level);

public:
	using GameSystem::GameSystem;
	void Give(
		const std::map<std::string, std::vector<std::string>>* level_groups,
		const std::map<int, std::map<std::string, float>>* level_completion_time_records,
		std::function<void(std::string)> set_level,
		std::function<void(bool)> set_edit_mode);
	void Update(Level& level, float dt);
};