#pragma once
#include "systems/level_menu.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"
#include "utils.hpp"
#include <iomanip>
#include <sstream>

#define LEVEL_PREVIEW_SCALE 0.6
#define BUTTON_HORISONTAL_MARGIN 0.06
#define BUTTON_VERTICAL_MARGIN 0.01
#define BUTTON_ASPECT_RATIO 8

void LevelMenuSystem::Give(
	const std::map<std::string, std::vector<std::string>>* level_groups,
	const std::map<int, std::map<std::string, float>>* level_completion_time_records,
	const std::map<std::string, int>* level_coin_records,
	std::function<Level&(std::string)> set_level,
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture)
{
	level_groups_ = level_groups;
	level_completion_time_records_ = level_completion_time_records;
	level_coin_records = level_coin_records;
	set_level_ = set_level;
	generate_level_texture_ = generate_level_texture;
}
void LevelMenuSystem::Update(Level& level, float dt)
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

void LevelMenuSystem::UpdateUI(Level& level, LevelMenuUI* ui)
{
	int button_i = 0;
	for (int button_entity_id : ui->button_entity_ids)
	{
		if (level.HasComponents<HoveredStartedThisFrame>(button_entity_id))
		{
			ui->at_level_id = level_groups_->at(ui->level_group)[button_i];
			*ui->level_image_identifier = generate_level_texture_(ui->at_level_id, unsigned(level.GetSize().x * LEVEL_PREVIEW_SCALE), unsigned(level.GetSize().y * LEVEL_PREVIEW_SCALE));
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
	GenerateStatsString(ui);
}

void LevelMenuSystem::SetupUI(Level& level, LevelMenuUI* ui)
{
	ui->level_group = ui->requested_level_group;
	auto level_size = level.GetSize();
	for (auto entity_id : ui->entity_ids)
	{
		level.DeleteEntity(entity_id);
	}
	ui->entity_ids.clear();
	ui->button_entity_ids.clear();

	float button_panel_center = level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE) / 2;
	float title_h = 350;
	auto [title_entity_id, title_text, title_draw_priority, title_position] = level.CreateEntitiyWith<Text, DrawPriority, Position>();
	ui->entity_ids.push_back(title_entity_id);
	title_draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
	title_text->content = GetGroupDisplayNameFromGroupName(ui->level_group);
	title_position->position = sf::Vector2f(button_panel_center, title_h / 2);

	{ // Dot indicator
		float h = 120;
		auto [entity_id, shader, draw_info, draw_priority, width_and_height, position] = level.CreateEntitiyWith<Shader, DrawInfo, DrawPriority, WidthAndHeight, Position>();
		ui->entity_ids.push_back(entity_id);
		shader->fragment_shader_path = "shaders\\dots_indicator.frag";
		shader->int_uniforms["n_dots"] = level_groups_->size();
		shader->int_uniforms["active_dot"] = std::distance(level_groups_->cbegin(), level_groups_->find(ui->level_group));
		position->position.x = button_panel_center;
		position->position.y = title_h - h / 2;
		width_and_height->width_and_height.x = 400;
		width_and_height->width_and_height.y = h;
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
	}

	for (int p : { -1, 1 })
	{
		auto [nav_btn_entity_id, nav_btn_draw_info, nav_btn_text, nav_btn_draw_priority, nav_btn_w_h, nav_btn_position, nav_btn_receives_mouse_events, nav_btn_shortcut_key] = level.CreateEntitiyWith<DrawInfo, Text, DrawPriority, WidthAndHeight, Position, ReceivesButtonEvents, ShortcutKey>();
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
		nav_btn_draw_info->scale_to_fit = true;
		nav_btn_w_h->width_and_height = sf::Vector2f(1, 1) * float(title_h - 2 * level.GetSize().x * BUTTON_VERTICAL_MARGIN);
		nav_btn_text->content = p < 0 ? "<" : ">";
		nav_btn_position->position = sf::Vector2f(button_panel_center + p * 0.7 * button_panel_center, title_h / 2);
	}
	const float BUTTON_HEIGHT = float(BLOCK_SIZE);
	// Scroll window
	ScrollWindow* scroll_window;
	{
		auto [entity_id, scroll_window_local, width_and_height, position] = level.CreateEntitiyWith<ScrollWindow, WidthAndHeight, Position>();
		ui->entity_ids.push_back(entity_id);
		scroll_window = scroll_window_local;
		scroll_window->entity_height = BUTTON_HEIGHT;
		width_and_height->width_and_height = level_size;
		width_and_height->width_and_height.y -= title_h;
		position->position = level_size / 2.f;
		position->position.y += title_h / 2;
	}

	// Level buttons
	std::vector<std::function<void(void)>> button_functions = {};
	std::vector<std::string> button_texts = {};
	int i = 0;
	for (auto& level_id : (*level_groups_).at(ui->level_group))
	{
		sf::Vector2f button_position = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE) / 2, title_h + (0.5 + 1.5 * i) * float(BLOCK_SIZE));
		{ // Button
			auto [entity_id, on_released_this_frame, shader, width_and_height, position] = level.AddBlueprintAddComponents<OnReleasedThisFrame, Shader, WidthAndHeight, Position>("BPMenuNavigationButton");
			ui->entity_ids.push_back(entity_id);
			ui->button_entity_ids.push_back(entity_id);
			scroll_window->entities.push_back(entity_id);

			on_released_this_frame->func = std::bind(&LevelMenuSystem::EnterLevel, this, level_id);
			width_and_height->width_and_height = sf::Vector2f(10, 1) * float(BLOCK_SIZE);
			position->position = button_position;
		}
		{ // Text
			auto [entity_id, text, draw_priority, shader, position] = level.CreateEntitiyWith<Text, DrawPriority, Shader, Position>();
			ui->entity_ids.push_back(entity_id);
			scroll_window->entities.push_back(entity_id);
			position->position = button_position;
			shader->fragment_shader_path = "shaders\\scroll.frag";
			text->content = GetLevelDisplayNameFromId(level_id);
			text->size = 75;
			text->apply_shader = true;
			draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY + 1;
		}
		i++;
	}

	{ // Level preview
		auto [entity_id, draw_info, draw_priority, width_and_height, position] = level.CreateEntitiyWith<DrawInfo, DrawPriority, WidthAndHeight, Position>();
		ui->entity_ids.push_back(entity_id);
		position->position = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE / 2), level.GetSize().y * (LEVEL_PREVIEW_SCALE / 2));
		width_and_height->width_and_height = sf::Vector2f(level.GetSize().x * LEVEL_PREVIEW_SCALE, level.GetSize().y * LEVEL_PREVIEW_SCALE);
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		draw_info->scale_to_fit = true;
		ui->level_image_identifier = &draw_info->image_path;
	}

	{ // Stats display
		auto [entity_id, text, draw_priority, position] = level.CreateEntitiyWith<Text, DrawPriority, Position>();
		ui->entity_ids.push_back(entity_id);
		text->size = 140;
		ui->stats_string = &text->content;
		position->position = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE / 2), level.GetSize().y * (LEVEL_PREVIEW_SCALE));
	}
}

void LevelMenuSystem::EnterLevel(std::string level_id)
{
	Level& entered_level = set_level_(level_id);
	if (is_in_level_editing_)
	{
		entered_level.SetMode(EDIT_MODE);
	}
}

void LevelMenuSystem::GenerateStatsString(LevelMenuUI* ui)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2);
	ss << "\nBest completion times:\n";
	for (const auto& [coin_n, records] : *level_completion_time_records_)
	{
		ss << "   " << std::to_string(coin_n) << " coins: ";
		if (records.count(ui->at_level_id) > 0)
		{
			ss << records.at(ui->at_level_id);
		}
		else
		{
			ss << "-";
		}
		ss << "\n";
	}
	*ui->stats_string = ss.str();
}