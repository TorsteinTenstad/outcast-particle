#pragma once
#include "game_system.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"

class LevelMenuSystem : public GameSystem
{
private:
	const std::map<std::string, std::vector<std::string>>* level_groups_;
	const std::map<std::string, float>* level_completion_time_records_;
	const std::map<std::string, int>* level_coin_records_;

	std::function<void(std::string)> set_level_;

public:
	using GameSystem::GameSystem;
	void Give(
		const std::map<std::string, std::vector<std::string>>* level_groups,
		const std::map<std::string, float>* level_completion_time_records,
		const std::map<std::string, int>* level_coin_records,
		std::function<void(std::string)> set_level)
	{
		level_groups_ = level_groups;
		level_completion_time_records_ = level_completion_time_records;
		level_coin_records = level_coin_records;
		set_level_ = set_level;
	}
	void Update(Level& level, float dt)
	{
		auto level_menu_ui_entities = level.GetEntitiesWith<LevelMenuUI>();
		assert(!(level_menu_ui_entities.size() > 1));
		if (level_menu_ui_entities.size() == 0)
		{
			return;
		}
		auto [ui_entity_id, ui] = level_menu_ui_entities[0];

		if (!ui->initialized)
		{
			Initialize(level, ui);
		}
	}
	void Initialize(Level& level, LevelMenuUI* level_menu_ui)
	{
		int i = 0;
		for (auto& level_id : level_groups_->cbegin()->second)
		{
			float x = 3000;
			float y = 400 + 500 * i;
			int id = AddMenuButton(level, std::bind(set_level_, level_id), x, y, level_id);
			i++;
		}
		level.AddBlueprint("BPMenuNavigator");

		level_menu_ui->initialized = true;
	}
};