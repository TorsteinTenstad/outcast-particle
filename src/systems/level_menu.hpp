#pragma once
#include "game_system.hpp"
#include "level.hpp"
#include "utils/string_parsing.hpp"

#include <iomanip>
#include <sstream>

#define LEVEL_PREVIEW_SCALE 0.6
#define BUTTON_HORISONTAL_MARGIN 0.06
#define BUTTON_VERTICAL_MARGIN 0.01
#define BUTTON_ASPECT_RATIO 8

class LevelMenuSystem : public GameSystem
{
private:
	const std::map<std::string, std::vector<std::string>>* level_groups_;
	const std::map<int, std::map<std::string, float>>* level_completion_time_records_;
	const std::map<std::string, int>* level_coin_records_;

	std::function<Level&(std::string)> set_level_;
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture_;

public:
	using GameSystem::GameSystem;
	void Give(
		const std::map<std::string, std::vector<std::string>>* level_groups,
		const std::map<int, std::map<std::string, float>>* level_completion_time_records,
		const std::map<std::string, int>* level_coin_records,
		std::function<Level&(std::string)> set_level,
		std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture);
	void Update(Level& level, float dt);
	void UpdateUI(Level& level, LevelMenuUI* ui);
	void SetupUI(Level& level, LevelMenuUI* ui);
	void GenerateStatsBadges(Level& level, LevelMenuUI* ui);
};