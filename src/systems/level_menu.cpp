#pragma once
#include "components/level_menu.hpp"
#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/menu_navigator.hpp"
#include "components/position.hpp"
#include "components/scroll.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "entity_creation.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "systems/level_menu.hpp"
#include "utils/container_operations.hpp"
#include "utils/level_id.hpp"
#include "utils/string_manip.hpp"
#include <cassert>
#include <iomanip>
#include <sstream>

#define LEVEL_PREVIEW_SCALE 0.6

void LevelMenuSystem::Give(
	const std::map<std::string, std::vector<std::string>>* level_groups,
	const std::map<int, std::map<std::string, float>>* level_completion_time_records,
	const std::map<std::string, int>* level_coin_records,
	std::function<Level&(std::string)> set_level,
	std::function<std::string(std::string)> create_new_level,
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture)
{
	level_groups_ = level_groups;
	create_new_level_ = create_new_level;
	level_completion_time_records_ = level_completion_time_records;
	level_coin_records = level_coin_records;
	set_level_ = set_level;
	generate_level_texture_ = generate_level_texture;
}
void LevelMenuSystem::Update(Level& level, float dt)
{
	auto level_menu_ui_entities = level.GetEntitiesWith<LevelMenuUI>();
	if (level_menu_ui_entities.size() == 0)
	{
		return;
	}
	assert(level_menu_ui_entities.size() < 3); // Is allowed to be 2 for one frame, when the ui is re-setup
	auto [entity_id, ui] = level_menu_ui_entities.back();
	for (const auto [id, _] : level_menu_ui_entities)
	{
		if (entity_id != id)
		{
			level.DeleteEntitiesWith<Position>();
			level.DeleteEntity(id);
		}
	}

	if (!ui->initialized)
	{
		if (ui->at_level_id.empty())
		{
			if (ui->last_at_level_id.empty())
			{
				ui->at_level_id = level_groups_->begin()->second[0];
			}
			else
			{
				ui->at_level_id = ui->last_at_level_id;
			}
		}
		SetupUI(level, ui);
		ui->initialized = true;
	}

	UpdateUI(level, ui);
}

void LevelMenuSystem::UpdateUI(Level& level, LevelMenuUI* ui)
{
	std::string level_group = GetGroupNameFromId(ui->at_level_id);
	int button_i = 0;
	for (int button_entity_id : ui->button_entity_ids)
	{
		if (level.HasComponents<HoveredStartedThisFrame>(button_entity_id))
		{
			if (ui->at_level_id != level_groups_->at(level_group)[button_i])
			{
				ui->at_level_id = level_groups_->at(level_group)[button_i];
				*ui->level_image_identifier = generate_level_texture_(ui->at_level_id, unsigned(level.GetSize().x * LEVEL_PREVIEW_SCALE), unsigned(level.GetSize().y * LEVEL_PREVIEW_SCALE));
			}
		}
		button_i++;
	}
	std::string new_level_id = level_group;
	if (level.HasComponents<ReleasedThisFrame>(ui->next_group_button_id))
	{
		new_level_id = NextInMap(*level_groups_, level_group)->second[0];
	}
	if (level.HasComponents<ReleasedThisFrame>(ui->prev_group_button_id))
	{
		new_level_id = PrevInMap(*level_groups_, level_group)->second[0];
	}
	if (level.HasComponents<ReleasedThisFrame>(ui->dot_indicator_id))
	{
		float pos_x = level.GetComponent<Position>(ui->dot_indicator_id)->position.x;
		float w = level.GetComponent<WidthAndHeight>(ui->dot_indicator_id)->width_and_height.x;
		float mouse_x = cursor_and_keys_.cursor_position.x;
		float percentage = (mouse_x - pos_x + w / 2) / w;

		if (0 <= percentage && percentage <= 1)
		{
			int n = level_groups_->size();
			int i = floor(percentage * n);

			auto it = level_groups_->begin();
			std::advance(it, i);
			new_level_id = it->second[0];
		}
	}
	if (new_level_id != level_group)
	{
		auto [new_id, new_ui] = level.CreateEntityWith<LevelMenuUI>();
		new_ui->at_level_id = new_level_id;
	}
	GenerateStatsBadges(level, ui);
}

void LevelMenuSystem::SetupUI(Level& level, LevelMenuUI* ui)
{
	std::string level_group = GetGroupNameFromId(ui->at_level_id);
	auto level_size = level.GetSize();
	assert(!ui->initialized);
	assert(ui->button_entity_ids.size() == 0);

	float button_panel_center = level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE) / 2;
	float title_h = 3 * float(BLOCK_SIZE);

	float dot_indicator_h = 0.5 * float(BLOCK_SIZE);
	{ // Dot indicator
		auto [entity_id, shader, receives_button_events, draw_info, draw_priority, width_and_height, position] = level.CreateEntityWith<Shader, ReceivesButtonEvents, DrawInfo, DrawPriority, WidthAndHeight, Position>();
		ui->dot_indicator_id = entity_id;
		shader->fragment_shader_path = "shaders\\dots_indicator.frag";
		shader->int_uniforms["n_dots"] = level_groups_->size();
		shader->int_uniforms["active_dot"] = std::distance(level_groups_->cbegin(), level_groups_->find(level_group));
		position->position.x = button_panel_center;
		position->position.y = title_h - dot_indicator_h / 2 - 0.5 * float(BLOCK_SIZE);
		width_and_height->width_and_height.x = 400;
		width_and_height->width_and_height.y = dot_indicator_h;
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
	}
	{ // Title
		auto [entity_id, text, draw_priority, position] = level.CreateEntityWith<Text, DrawPriority, Position>();
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		text->size = 100;
		text->content = GetGroupDisplayNameFromGroupName(level_group);
		position->position = sf::Vector2f(button_panel_center, title_h / 2 - 0.5 * dot_indicator_h);
	}

	// Group navigation buttons
	for (int p : { -1, 1 })
	{
		auto [entity_id, draw_info, shader, fill_color, mouse_interaction_dependent_fill_color, text, draw_priority, width_and_height, position, receives_mouse_events, shortcut_key] =
			level.CreateEntityWith<DrawInfo, Shader, FillColor, MouseInteractionDependentFillColor, Text, DrawPriority, WidthAndHeight, Position, ReceivesButtonEvents, ShortcutKey>();

		if (p == -1)
		{
			ui->prev_group_button_id = entity_id;
			shortcut_key->key = sf::Keyboard::Key::Left;
		}
		if (p == 1)
		{
			ui->next_group_button_id = entity_id;
			shortcut_key->key = sf::Keyboard::Key::Right;
		}
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		draw_info->image_path = "content\\textures\\white.png";
		width_and_height->width_and_height = sf::Vector2f(2, 2) * float(BLOCK_SIZE);
		text->content = p < 0 ? "<" : ">";
		shader->fragment_shader_path = "shaders\\round_corners.frag";
		position->position = sf::Vector2f(button_panel_center + p * (4) * float(BLOCK_SIZE), title_h / 2);
	}
	const float BUTTON_HEIGHT = float(BLOCK_SIZE);
	// Scroll window
	ScrollWindow* scroll_window;
	{
		auto [entity_id, scroll_window_local, width_and_height, position] = level.CreateEntityWith<ScrollWindow, WidthAndHeight, Position>();

		scroll_window = scroll_window_local;
		scroll_window->entity_height = BUTTON_HEIGHT;
		auto [menu_navigator_id, _] = CreateMenuNavigator(level, 1);
		scroll_window->menu_navigator = menu_navigator_id;
		width_and_height->width_and_height = level_size;
		width_and_height->width_and_height.y -= title_h;
		position->position = level_size / 2.f;
		position->position.y += title_h / 2;
	}

	// Level buttons
	auto AddButton = [&level, this, ui, scroll_window, title_h](int i, std::string button_text, std::function<void()> button_function) {
		sf::Vector2f button_position = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE) / 2, title_h + (0.5 + 1.5 * i) * float(BLOCK_SIZE));
		{ // Text
			auto [id, size] = CreateScrollingText(level, button_position, button_text);
			level.GetComponent<Text>(id)->size = 75;
			scroll_window->entities.push_back(id);
		}
		{ // Button
			auto [id, size] = CreateNavigatorButton(level, button_position, button_function, "", sf::Keyboard::Unknown);
			scroll_window->entities.push_back(id);

			level.GetComponent<WidthAndHeight>(id)->width_and_height = sf::Vector2f(10, 1) * float(BLOCK_SIZE);
			level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
			return id;
		}
	};

	int i = 0;
	for (auto& level_id : (*level_groups_).at(level_group))
	{
		int id = AddButton(i, GetLevelDisplayNameFromId(level_id), std::bind(&LevelMenuSystem::EnterLevel, this, level_id));
		ui->button_entity_ids.push_back(id);
		if (level_id == ui->at_level_id)
		{
			level.GetComponent<MenuNavigator>(scroll_window->menu_navigator.value())->currently_at_entity_id = id;
		}
		i++;
	}
	if (is_in_level_editing_)
	{
		AddButton(i, "+", std::bind(&LevelMenuSystem::EnterLevel, this, (std::bind(create_new_level_, level_group))));
	}

	{ // Level preview
		auto [entity_id, draw_info, draw_priority, width_and_height, position] = level.CreateEntityWith<DrawInfo, DrawPriority, WidthAndHeight, Position>();

		position->position = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE / 2), level.GetSize().y * (LEVEL_PREVIEW_SCALE / 2));
		width_and_height->width_and_height = sf::Vector2f(level.GetSize().x * LEVEL_PREVIEW_SCALE, level.GetSize().y * LEVEL_PREVIEW_SCALE);
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		draw_info->scale_to_fit = true;
		ui->level_image_identifier = &draw_info->image_path;
		*ui->level_image_identifier = generate_level_texture_(ui->at_level_id, unsigned(level.GetSize().x * LEVEL_PREVIEW_SCALE), unsigned(level.GetSize().y * LEVEL_PREVIEW_SCALE));
	}

	{ // Stats display
		sf::Vector2f badge_center_positions = sf::Vector2f(level_size.x * (1 - LEVEL_PREVIEW_SCALE) + 3.75 * BLOCK_SIZE, level_size.y * (0.5 + 0.5 * LEVEL_PREVIEW_SCALE));
		std::vector<entities_handle> entities_handles;
		for (int i = 0; i < 4; i++)
		{
			entities_handles.push_back(AdaptToEntitiesHandle(CreateStatsBadge(level, sf::Vector2f(0, 0), i, 50, "", false)));
		}
		auto [ids, heights] = VerticalEntityLayout(level, badge_center_positions, entities_handles, BLOCK_SIZE / 4);
		ui->stats_block_ids = ids;
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

void LevelMenuSystem::GenerateStatsBadges(Level& level, LevelMenuUI* ui)
{
	for (int i = 0; i < 4; i++)
	{
		if ((*level_completion_time_records_).count(i) == 0)
		{
			continue;
		}
		std::map<std::string, float> i_coin_record = (*level_completion_time_records_).at(i);
		if (i_coin_record.count(ui->at_level_id) < 1)
		{
			level.GetComponent<FillColor>(ui->stats_block_ids[i])->color.a = 0;
			level.GetComponent<Text>(ui->stats_block_ids[i])->content = "";
		}
		else
		{
			level.GetComponent<FillColor>(ui->stats_block_ids[i])->color.a = 255;
			level.GetComponent<Text>(ui->stats_block_ids[i])->content = RightShiftString(CreateBadgeText(i_coin_record.at(ui->at_level_id), 2 + globals.general_config.display_precise_badge_time), 16);
		}
	}
}