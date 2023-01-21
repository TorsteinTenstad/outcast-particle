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
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture_;

public:
	using GameSystem::GameSystem;
	void Give(
		const std::map<std::string, std::vector<std::string>>* level_groups,
		const std::map<std::string, float>* level_completion_time_records,
		const std::map<std::string, int>* level_coin_records,
		std::function<void(std::string)> set_level,
		std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture)
	{
		level_groups_ = level_groups;
		level_completion_time_records_ = level_completion_time_records;
		level_coin_records = level_coin_records;
		set_level_ = set_level;
		generate_level_texture_ = generate_level_texture;
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

		UpdateUI(level, ui);
	}

	void UpdateUI(Level& level, LevelMenuUI* ui)
	{
		int button_i = 0;
		for (int button_entity_id : ui->button_entity_ids)
		{
			if (level.HasComponents<HoveredStartedThisFrame>(button_entity_id))
			{
				*ui->level_image_identifier = generate_level_texture_(level_groups_->at(ui->level_group)[button_i], 3200, 1800);
			}
			button_i++;
		}
	}

	void Initialize(Level& level, LevelMenuUI* ui)
	{
		int i = 0;
		for (auto& level_id : level_groups_->cbegin()->second)
		{
			float x = 2000;
			float y = 400 + 500 * i;
			int id = AddMenuButton(level, std::bind(set_level_, level_id), x, y, GetLevelNameFromId(level_id));
			ui->button_entity_ids.push_back(id);
			i++;
		}
		level.AddBlueprint("BPMenuNavigator");
		auto [image_entity_id, draw_info, draw_priority, width_and_height, position] = level.CreateEntitiyWith<DrawInfo, DrawPriority, WidthAndHeight, Position>();
		position->position = sf::Vector2f(6000, level.size.y / 2);
		width_and_height->width_and_height = sf::Vector2f(3200, 1800);
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		draw_info->scale_to_fit = false;
		ui->level_image_identifier = &draw_info->image_path;
		ui->level_group = level_groups_->begin()->first;
		ui->initialized = true;
	}
};