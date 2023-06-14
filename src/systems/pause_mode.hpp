#include "game_system.hpp"
#include "records_manager.hpp"
#include <algorithm>
#include <functional>
#include <string>

class PauseMode : public GameSystem
{
private:
	std::function<Level&(std::string)> set_level_;
	const std::map<std::string, std::vector<std::string>>* level_groups_;
	const RecordsManager* records_;

	void SetupPauseMenu(Level& level, LevelMode previous_mode);

public:
	using GameSystem::GameSystem;
	void Give(
		std::function<Level&(std::string)> set_level,
		const std::map<std::string, std::vector<std::string>>* level_groups,
		const RecordsManager* records);
	void Update(Level& level, float dt);
};