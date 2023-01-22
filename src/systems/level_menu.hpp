#pragma once
#include "game_system.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"
#include "utils.hpp"

#define LEVEL_PREVIEW_WIDTH 0.6
#define LEVEL_PREVIEW_HEIGHT (0.6 / ASPECT_RATIO)
#define BUTTON_HORISONTAL_MARGIN 0.06
#define BUTTON_VERTICAL_MARGIN 0.01
#define BUTTON_ASPECT_RATIO 6

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
			ui->requested_level_group = level_groups_->begin()->first;
			ui->initialized = true;
		}
		if (ui->level_group != ui->requested_level_group)
		{
			SetupUI(level, ui);
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
				*ui->level_image_identifier = generate_level_texture_(level_groups_->at(ui->level_group)[button_i], unsigned(level.size.x * LEVEL_PREVIEW_WIDTH), unsigned(level.size.x * LEVEL_PREVIEW_HEIGHT));
			}
			button_i++;
		}
		if (level.HasComponents<ReleasedThisFrame>(ui->next_group_button_id))
		{
			ui->requested_level_group = NextKey(*level_groups_, ui->level_group);
		}
		if (level.HasComponents<ReleasedThisFrame>(ui->prev_group_button_id))
		{
			ui->requested_level_group = PrevKey(*level_groups_, ui->level_group);
		}
	}

	void SetupUI(Level& level, LevelMenuUI* ui)
	{
		ui->level_group = ui->requested_level_group;

		for (auto entity_id : ui->entity_ids)
		{
			level.DeleteEntity(entity_id);
		}
		ui->entity_ids.clear();
		ui->button_entity_ids.clear();

		float button_panel_center = level.size.x * (1 - LEVEL_PREVIEW_WIDTH) / 2;
		float title_h = 400;
		auto [title_entity_id, title_text, title_draw_priority, title_position] = level.CreateEntitiyWith<Text, DrawPriority, Position>();
		ui->entity_ids.push_back(title_entity_id);
		title_draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		title_text->content = GetGroupDisplayNameFromGroupName(ui->level_group);
		title_position->position = sf::Vector2f(button_panel_center, title_h / 2);

		for (int p : { -1, 1 })
		{
			auto [nav_btn_entity_id, nav_btn_draw_info, nav_btn_text, nav_btn_draw_priority, nav_btn_w_h, nav_btn_position, nav_btn_receives_mouse_events, nav_btn_shortcut_key] = level.CreateEntitiyWith<DrawInfo, Text, DrawPriority, WidthAndHeight, Position, ReceivesMouseEvents, ShortcutKey>();
			ui->entity_ids.push_back(nav_btn_entity_id);
			if (p == -1)
			{
				ui->prev_group_button_id = nav_btn_entity_id;
				nav_btn_shortcut_key->key = sf::Keyboard::Key::Left;
			}
			if (p == 1)
			{
				ui->next_group_button_id = nav_btn_entity_id;
				nav_btn_shortcut_key->key = sf::Keyboard::Key::Right;
			}
			nav_btn_draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
			nav_btn_draw_info->image_path = "content\\textures_generated\\button_500_500.png";
			nav_btn_w_h->width_and_height = sf::Vector2f(1, 1) * float(title_h - 2 * level.size.x * BUTTON_VERTICAL_MARGIN);
			nav_btn_text->content = p < 0 ? "<" : ">";
			nav_btn_position->position = sf::Vector2f(button_panel_center + p * 0.7 * button_panel_center, title_h / 2);
		}

		int i = 0;
		for (auto& level_id : (*level_groups_).at(ui->level_group))
		{
			float w = level.size.x * (1 - LEVEL_PREVIEW_WIDTH - 2 * BUTTON_HORISONTAL_MARGIN);
			float h = w / BUTTON_ASPECT_RATIO;
			float x = button_panel_center;
			float y = title_h + h / 2 + level.size.x * BUTTON_VERTICAL_MARGIN + i * (h + level.size.x * BUTTON_VERTICAL_MARGIN);
			int id = AddMenuButton(level, std::bind(set_level_, level_id), x, y, GetLevelDisplayNameFromId(level_id));
			ui->entity_ids.push_back(id);
			ui->button_entity_ids.push_back(id);
			level.GetComponent<WidthAndHeight>()[id].width_and_height = sf::Vector2f(w, h);
			i++;
		}
		int menu_button_navigator_id = level.AddBlueprint("BPMenuNavigator");
		ui->entity_ids.push_back(menu_button_navigator_id);
		auto [level_preview_entity_id, draw_info, draw_priority, width_and_height, position] = level.CreateEntitiyWith<DrawInfo, DrawPriority, WidthAndHeight, Position>();
		ui->entity_ids.push_back(level_preview_entity_id);
		position->position = sf::Vector2f(level.size.x * (1 - LEVEL_PREVIEW_WIDTH / 2), level.size.x * (LEVEL_PREVIEW_HEIGHT / 2));
		width_and_height->width_and_height = sf::Vector2f(level.size.x * LEVEL_PREVIEW_WIDTH, level.size.x * LEVEL_PREVIEW_HEIGHT);
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		draw_info->scale_to_fit = true;
		ui->level_image_identifier = &draw_info->image_path;
	}
};