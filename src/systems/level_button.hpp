#pragma once
#include "game_system.hpp"
#include "level.hpp"
#include "modes.hpp"
#include "string_parsing_utils.hpp"

class LevelButtonSystem : public GameSystem
{
private:
	std::map<int, float>* level_completion_time_records_;

public:
	using GameSystem::GameSystem;
	void SetLevelCompletionTimeRecords(std::map<int, float>* level_completion_time_records)
	{
		level_completion_time_records_ = level_completion_time_records;
	}
	void Update(Level& level, float dt)
	{
		auto& level_button_map = level.GetComponent<LevelButton>();
		auto& border_map = level.GetComponent<Border>();
		auto& text_map = level.GetComponent<Text>();
		{
			for (auto& [entity_id, level_button] : level_button_map)
			{
				if ((*level_completion_time_records_)[level_button.level] > 0)
				{
					std::stringstream ss;
					ss << std::fixed << std::setprecision(2) << (*level_completion_time_records_)[level_button.level];
					text_map[entity_id].content = ss.str();
					text_map[entity_id].color = sf::Color(255, 255, 255, 200);
					text_map[entity_id].size = 500;
				}
			}
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};