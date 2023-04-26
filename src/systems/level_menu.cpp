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
	LevelManager* level_manager,
	const std::map<int, std::map<std::string, float>>* level_completion_time_records,
	const std::map<std::string, int>* level_coin_records,
	std::function<Level&(std::string)> set_level,
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture)
{
	level_manager_ = level_manager;
	level_completion_time_records_ = level_completion_time_records;
	level_coin_records = level_coin_records;
	set_level_ = set_level;
	generate_level_texture_ = generate_level_texture;
}
void LevelMenuSystem::Update(Level& level, float dt)
{
	const std::map<std::string, std::vector<std::string>>& level_groups = level_manager_->GetLevels();

	LevelMenuUI* ui = level.FindSingleton<LevelMenuUI>();
	if (!ui)
	{
		return;
	}

	if (!ui->initialized)
	{
		SetupUI(level, ui);
		ui->initialized = true;
	}

	bool redraw_ui = UpdateUI(level, ui);

	if (redraw_ui)
	{
		level.Clear();
		auto [entity_id, ui] = level.CreateEntityWith<LevelMenuUI>();
	}
}

bool LevelMenuSystem::UpdateUI(Level& level, LevelMenuUI* ui)
{
	const std::map<std::string, std::vector<std::string>>& level_groups = level_manager_->GetLevels();
	std::string at_group = ui->at_group.value();

	{ // Updates that don't require redraw, only modifications:

		for (auto [button_entity_id, level_id] : zip(ui->button_entity_ids, level_groups.at(at_group)))
		{
			if (!level.HasComponents<HoveredStartedThisFrame>(button_entity_id)) { continue; }
			if (ui->at_level_id.has_value() && ui->at_level_id.value() == level_id) { continue; }

			ui->at_level_id = level_id;
			unsigned texture_size_x = unsigned(level.GetSize().x * LEVEL_PREVIEW_SCALE);
			unsigned texture_size_y = unsigned(level.GetSize().y * LEVEL_PREVIEW_SCALE);
			std::string texture_identifier = generate_level_texture_(level_id, texture_size_x, texture_size_y);
			level.GetComponent<DrawInfo>(ui->level_preview_id)->image_path = texture_identifier;
			UpdateStatsBadges(level, ui);
		}
	}

	{ // Check for updates that require redraw:
		std::optional<std::string> new_level_id;
		if (level.HasComponents<ReleasedThisFrame>(ui->next_group_button_id))
		{
			new_level_id = NextInMap(level_groups, at_group)->second[0];
		}
		if (level.HasComponents<ReleasedThisFrame>(ui->prev_group_button_id))
		{
			new_level_id = PrevInMap(level_groups, at_group)->second[0];
		}
		if (level.HasComponents<ReleasedThisFrame>(ui->dot_indicator_id))
		{
			float pos_x = level.GetComponent<Position>(ui->dot_indicator_id)->position.x;
			float w = level.GetComponent<WidthAndHeight>(ui->dot_indicator_id)->width_and_height.x;
			float mouse_x = cursor_and_keys_.cursor_position.x;
			float percentage = (mouse_x - pos_x + w / 2) / w;

			if (0 <= percentage && percentage <= 1)
			{
				int n = level_groups.size();
				int i = floor(percentage * n);

				auto it = level_groups.begin();
				std::advance(it, i);
				new_level_id = it->second[0];
			}
		}
		if (new_level_id && GetGroupNameFromId(new_level_id.value()) != at_group)
		{
			auto [new_id, new_ui] = level.CreateEntityWith<LevelMenuUI>();
			new_ui->at_level_id = new_level_id.value();
			return true;
		}

		int i = 0;
		for (int button_id : ui->delete_level_button_entity_ids)
		{
			if (level.HasComponents<ReleasedThisFrame>(button_id))
			{
				level_manager_->DeleteLevel(level_groups.at(at_group)[i]);
				return true;
			}
			i++;
		}
	}
	return false;
}

void LevelMenuSystem::SetupUI(Level& level, LevelMenuUI* ui)
{
	const std::map<std::string, std::vector<std::string>>& level_groups = level_manager_->GetLevels();
	if (!ui->at_group.has_value())
	{
		ui->at_group = level_groups.begin()->first;
	}
	std::string at_group = ui->at_group.value();
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
		shader->int_uniforms["n_dots"] = level_groups.size();
		shader->int_uniforms["active_dot"] = std::distance(level_groups.cbegin(), level_groups.find(at_group));
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
		text->content = GetGroupDisplayNameFromGroupName(at_group);
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
	auto [menu_navigator_id, _] = CreateMenuNavigator(level, 1);
	ScrollWindow* scroll_window;
	{
		auto [entity_id, scroll_window_local, width_and_height, position] = level.CreateEntityWith<ScrollWindow, WidthAndHeight, Position>();

		scroll_window = scroll_window_local;
		scroll_window->entity_height = BUTTON_HEIGHT;
		scroll_window->menu_navigator = menu_navigator_id;
		width_and_height->width_and_height = level_size;
		width_and_height->width_and_height.y -= title_h;
		position->position = level_size / 2.f;
		position->position.y += title_h / 2;
	}

	// Level buttons
	auto AddLevelMenuButton = [](ECSScene& level, std::string button_text, std::function<void()> button_function, float blocks_wide) {
		// Button
		auto [button_id, size] = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(blocks_wide, 1) * float(BLOCK_SIZE));
		level.AddComponent<OnReleasedThisFrame>(button_id)->func = button_function;
		level.AddComponent<MenuNavigable>(button_id);
		// Text
		auto [text_id, _] = CreateScrollingText(level, sf::Vector2f(0, 0), button_text, 75u);

		EntitiesHandle main_button = { { button_id, text_id }, size };
		return main_button;
	};

	auto AddLevelMenuRow = [ui, AddLevelMenuButton, is_in_level_editing = is_in_level_editing_](ECSScene& level, std::string button_text, std::function<void()> button_function) {
		std::vector<EntitiesHandle> row_items;
		EntitiesHandle main_button = AddLevelMenuButton(level, button_text, button_function, is_in_level_editing ? 7.5 : 10);
		ui->button_entity_ids.push_back(std::get<std::vector<int>>(main_button)[0]);
		if (!is_in_level_editing)
		{
			return main_button;
		}
		EntityHandle edit_name_button = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(1, 1) * float(BLOCK_SIZE));
		EntityHandle delete_level_button = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(1, 1) * float(BLOCK_SIZE));

		ui->edit_name_button_entity_ids.push_back(std::get<int>(edit_name_button));
		ui->delete_level_button_entity_ids.push_back(std::get<int>(delete_level_button));
		row_items.push_back(main_button);
		row_items.push_back(AdaptToEntitiesHandle(edit_name_button));
		row_items.push_back(AdaptToEntitiesHandle(delete_level_button));
		return HorizontalEntityLayout(level, sf::Vector2f(0, 0), row_items, 0.25 * float(BLOCK_SIZE));
	};

	std::vector<EntitiesHandle> scroll_menu_items;
	for (const auto& level_id : level_groups.at(at_group))
	{
		EntitiesHandle row = AddLevelMenuRow(level, GetLevelDisplayNameFromId(level_id), [this]() {

		});
		scroll_menu_items.push_back(row);
	}
	for (const auto& [button_id, level_id] : zip(ui->button_entity_ids, level_groups.at(at_group)))
	{
		if (ui->last_at_level_id == level_id)
		{
			level.GetComponent<MenuNavigator>(menu_navigator_id)->currently_at_entity_id = button_id;
			break;
		}
	}
	if (is_in_level_editing_)
	{
		EntitiesHandle new_level_button = AddLevelMenuButton(level, "+", std::bind(&LevelMenuSystem::EnterLevel, this, (std::bind(&LevelManager::CreateNewLevel, level_manager_, at_group))), 10);
		scroll_menu_items.push_back(new_level_button);
	}
	VerticalEntityLayout(level, sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE) / 2, title_h), scroll_menu_items, 0.5 * float(BLOCK_SIZE), StartEdge);

	{ // Level preview
		auto [entity_id, draw_info, draw_priority, width_and_height, position] = level.CreateEntityWith<DrawInfo, DrawPriority, WidthAndHeight, Position>();
		ui->level_preview_id = entity_id;
		position->position = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE / 2), level.GetSize().y * (LEVEL_PREVIEW_SCALE / 2));
		width_and_height->width_and_height = sf::Vector2f(level.GetSize().x * LEVEL_PREVIEW_SCALE, level.GetSize().y * LEVEL_PREVIEW_SCALE);
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		draw_info->scale_to_fit = true;

		if (ui->at_level_id.has_value())
		{
			std::string at_level_id = ui->at_level_id.value();
			unsigned texture_size_x = unsigned(level.GetSize().x * LEVEL_PREVIEW_SCALE);
			unsigned texture_size_y = unsigned(level.GetSize().y * LEVEL_PREVIEW_SCALE);
			std::string texture_identifier = generate_level_texture_(at_level_id, texture_size_x, texture_size_y);
		}
	}

	{ // Stats display
		sf::Vector2f badge_center_positions = sf::Vector2f(level_size.x * (1 - LEVEL_PREVIEW_SCALE) + 3.75 * BLOCK_SIZE, level_size.y * (0.5 + 0.5 * LEVEL_PREVIEW_SCALE));
		std::vector<EntitiesHandle> entities_handles;
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

void LevelMenuSystem::UpdateStatsBadges(Level& level, LevelMenuUI* ui)
{
	assert(ui->at_level_id.has_value());
	std::string at_level_id = ui->at_level_id.value();
	for (int i = 0; i < 4; i++)
	{
		if (level_completion_time_records_->count(i) == 0)
		{
			continue;
		}
		std::map<std::string, float> i_coin_record = level_completion_time_records_->at(i);
		if (i_coin_record.count(at_level_id) > 1)
		{
			level.GetComponent<FillColor>(ui->stats_block_ids[i])->color.a = 255;
			level.GetComponent<Text>(ui->stats_block_ids[i])->content = RightShiftString(CreateBadgeText(i_coin_record.at(at_level_id), 2 + globals.general_config.display_precise_badge_time), 16);
		}
		else
		{
			level.GetComponent<FillColor>(ui->stats_block_ids[i])->color.a = 0;
			level.GetComponent<Text>(ui->stats_block_ids[i])->content = "";
		}
	}
}