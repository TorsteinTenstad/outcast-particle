#include "game_system.hpp"
#include <algorithm>
#include <functional>
#include <string>

class PauseMode : public GameSystem
{
private:
	std::function<void(std::string)> set_level_;
	const std::map<std::string, std::vector<std::string>>* level_groups_;

	void SetupPauseMenu(Level& level, LevelMode previous_mode);
	void AddFloatingButtons(Level& level, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts, std::vector<sf::Keyboard::Key> shortcut_keys);

public:
	using GameSystem::GameSystem;
	void Give(std::function<void(std::string)> set_level, const std::map<std::string, std::vector<std::string>>* level_groups);
	void Update(Level& level, float dt);
};