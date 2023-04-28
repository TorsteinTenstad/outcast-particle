#pragma once
#include "components/level_menu.hpp"
#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/menu_navigator.hpp"
#include "components/position.hpp"
#include "components/scroll.hpp"
#include "components/size.hpp"
#include "components/sticky_button.hpp"
#include "components/text.hpp"
#include "entity_creation.hpp"
#include "folder_definitions.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "level_menu.hpp"
#include "systems/level_menu.hpp"
#include "utils/container_operations.hpp"
#include "utils/level_id.hpp"
#include "utils/string_manip.hpp"
#include <cassert>
#include <iomanip>
#include <sstream>

#define LEVEL_PREVIEW_SCALE 0.6

static void GenerateLevelPreview(Level& level, LevelMenuUI* ui,
	std::string level_id,
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture)
{
	unsigned texture_size_x = unsigned(level.GetSize().x * LEVEL_PREVIEW_SCALE);
	unsigned texture_size_y = unsigned(level.GetSize().y * LEVEL_PREVIEW_SCALE);
	std::string texture_identifier = generate_level_texture(level_id, texture_size_x, texture_size_y);
	level.GetComponent<DrawInfo>(ui->level_preview_id)->image_path = texture_identifier;
}

static void UpdateLevelPreview(Level& level, LevelMenuUI* ui,
	const std::map<std::string, std::vector<std::string>>& level_groups,
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture)
{
	assert(ui->at_group.has_value());
	std::string at_group = ui->at_group.value();
	for (const auto& [button_entity_id, level_id] : zip(ui->button_entity_ids, level_groups.at(at_group)))
	{
		if (!level.HasComponents<HoveredStartedThisFrame>(button_entity_id)) { continue; }
		if (ui->displaying_level_id.has_value() && ui->displaying_level_id.value() == level_id) { continue; }

		ui->displaying_level_id = level_id;
		ui->at_level_id = level_id;
		LevelMenuUI::last_at_level_id[at_group] = level_id;
		GenerateLevelPreview(level, ui, level_id, generate_level_texture);
		return;
	}
	if (ui->new_level_button_id.has_value()
		&& level.HasComponents<HoveredStartedThisFrame>(ui->new_level_button_id.value()))
	{
		GenerateLevelPreview(level, ui, NEW_LEVEL_TEMPLATE_FILE, generate_level_texture);
		ui->displaying_level_id = NEW_LEVEL_TEMPLATE_FILE;
	}
}

static void UpdateStatsBadges(Level& level, LevelMenuUI* ui,
	const std::map<int, std::map<std::string, float>>* level_completion_time_records)
{
	if (!ui->at_level_id.has_value())
	{
		return;
	}
	std::string at_level_id = ui->at_level_id.value();
	for (int i = 0; i < 4; i++)
	{
		if (level_completion_time_records->count(i) == 0)
		{
			continue;
		}
		std::map<std::string, float> i_coin_record = level_completion_time_records->at(i);
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

static void RequestRedraw(Level& level, LevelMenuUI* ui, std::string level_group, std::optional<std::string> at_level_id = std::nullopt)
{
	auto [entity_id, request] = level.CreateEntityWith<ReDrawLevelMenuRequest>();
	request->at_group = level_group;
	request->at_level_id = at_level_id;
}

static void RequestRedrawIfLevelGroupIsNew(Level& level, LevelMenuUI* ui, std::string new_group, std::optional<std::string> at_level_id = std::nullopt)
{

	assert(ui->at_group.has_value());
	std::string at_group = ui->at_group.value();
	if (new_group != at_group)
	{
		RequestRedraw(level, ui, new_group, at_level_id);
	}
}

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

	if (ReDrawLevelMenuRequest* request = level.FindSingleton<ReDrawLevelMenuRequest>())
	{
		auto at_group = request->at_group;
		auto at_level_id = request->at_level_id;
		level.Clear();
		auto [entity_id, ui] = level.CreateEntityWith<LevelMenuUI>();
		ui->at_group = at_group;
		ui->at_level_id = at_level_id;
		SetupUI(level, ui);
	}

	if (LevelMenuUI* ui = level.FindSingleton<LevelMenuUI>())
	{
		UpdateUI(level, ui);
	}
}

void LevelMenuSystem::UpdateUI(Level& level, LevelMenuUI* ui)
{
	const std::map<std::string, std::vector<std::string>>& level_groups = level_manager_->GetLevels();
	assert(ui->at_group.has_value());
	std::string at_group = ui->at_group.value();

	UpdateLevelPreview(level, ui, level_manager_->GetLevels(), generate_level_texture_);
	UpdateStatsBadges(level, ui, level_completion_time_records_);

	if (level.HasComponents<ReleasedThisFrame>(ui->next_group_button_id))
	{
		std::string new_group = NextInMap(level_groups, at_group)->first;
		return RequestRedrawIfLevelGroupIsNew(level, ui, new_group);
	}
	if (level.HasComponents<ReleasedThisFrame>(ui->prev_group_button_id))
	{
		std::string new_group = PrevInMap(level_groups, at_group)->first;
		return RequestRedrawIfLevelGroupIsNew(level, ui, new_group);
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
			return RequestRedrawIfLevelGroupIsNew(level, ui, it->first);
		}
	}
	if (ui->new_level_button_id.has_value() && level.HasComponents<ReleasedThisFrame>(ui->new_level_button_id.value()))
	{
		std::string new_level_id = level_manager_->CreateNewLevel(at_group);
		return RequestRedraw(level, ui, at_group, new_level_id);
	}

	if (ui->delete_level_button_entity_ids.size() > 0)
	{
		for (const auto& [main_button_id, text_id, edit_button_id, delete_button_id, level_id] : zip(
				 ui->button_entity_ids,
				 ui->text_entity_ids,
				 ui->edit_name_button_entity_ids,
				 ui->delete_level_button_entity_ids,
				 level_groups.at(at_group)))
		{
			if (level.HasComponents<HoveredStartedThisFrame>(edit_button_id) || level.HasComponents<HoveredStartedThisFrame>(delete_button_id))
			{
				level.GetComponent<MenuNavigator>(ui->menu_navigator_id)->currently_at_entity_id = main_button_id;
			}
			if (level.HasComponents<ReleasedThisFrame>(delete_button_id))
			{
				level_manager_->DeleteLevel(level_id);
				return RequestRedraw(level, ui, at_group);
			}
			if (level.HasComponents<StickyButtonDown>(edit_button_id) && !level.HasComponents<TextBox>(text_id))
			{
				level.AddComponents<TextBox>(text_id);
			}
			else if (!level.HasComponents<StickyButtonDown>(edit_button_id) && level.HasComponents<TextBox>(text_id))
			{
				level_manager_->RenameLevel(level_id, level.GetComponent<Text>(text_id)->content);
				level.RemoveComponents<TextBox>(text_id);
			}
		}
	}
}

void LevelMenuSystem::SetupUI(Level& level, LevelMenuUI* ui)
{
	assert(ui->button_entity_ids.size() == 0);

	const std::map<std::string, std::vector<std::string>>& level_groups = level_manager_->GetLevels();
	if (!ui->at_group.has_value())
	{
		ui->at_group = level_groups.begin()->first;
	}
	std::string at_group = ui->at_group.value();

	if (!ui->at_level_id.has_value())
	{
		std::vector<std::string> levels_in_group = level_groups.at(at_group);
		auto it = ui->last_at_level_id.find(at_group);
		if (it != ui->last_at_level_id.end())
		{
			ui->at_level_id = it->second;
		}
		else if (levels_in_group.size() > 0)
		{
			ui->at_level_id = levels_in_group[0];
		}
	}

	auto level_size = level.GetSize();

	const float BUTTONS_PANEL_CENTER = level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE) / 2;
	const float BUTTONS_WIDTH = 10 * float(BLOCK_SIZE);
	const float BUTTONS_HEIGHT = 1 * float(BLOCK_SIZE);
	const float BUTTONS_SPACING = 0.5 * float(BLOCK_SIZE);
	const float TITLE_H = 3 * float(BLOCK_SIZE);

	float dot_indicator_h = 0.5 * float(BLOCK_SIZE);
	{ // Dot indicator
		auto [entity_id, shader, receives_button_events, draw_info, draw_priority, width_and_height, position] = level.CreateEntityWith<Shader, ReceivesButtonEvents, DrawInfo, DrawPriority, WidthAndHeight, Position>();
		ui->dot_indicator_id = entity_id;
		shader->fragment_shader_path = "shaders\\dots_indicator.frag";
		shader->int_uniforms["n_dots"] = level_groups.size();
		shader->int_uniforms["active_dot"] = std::distance(level_groups.cbegin(), level_groups.find(at_group));
		position->position.x = BUTTONS_PANEL_CENTER;
		position->position.y = TITLE_H - dot_indicator_h / 2 - 0.5 * float(BLOCK_SIZE);
		width_and_height->width_and_height.x = 400;
		width_and_height->width_and_height.y = dot_indicator_h;
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
	}
	{ // Title
		auto [entity_id, text, draw_priority, position] = level.CreateEntityWith<Text, DrawPriority, Position>();
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		text->size = 100;
		text->content = GetGroupDisplayNameFromGroupName(at_group);
		position->position = sf::Vector2f(BUTTONS_PANEL_CENTER, TITLE_H / 2 - 0.5 * dot_indicator_h);
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
		position->position = sf::Vector2f(BUTTONS_PANEL_CENTER + p * (4) * float(BLOCK_SIZE), TITLE_H / 2);
	}
	// Scroll window
	auto [menu_navigator_id, _] = CreateMenuNavigator(level, 1);
	ui->menu_navigator_id = menu_navigator_id;
	ScrollWindow* scroll_window;
	{
		auto [entity_id, scroll_window_local, width_and_height, position] = level.CreateEntityWith<ScrollWindow, WidthAndHeight, Position>();
		scroll_window = scroll_window_local;
		scroll_window->entity_height = BUTTONS_HEIGHT;
		scroll_window->menu_navigator = menu_navigator_id;
		width_and_height->width_and_height = sf::Vector2f(BUTTONS_WIDTH, level_size.y - TITLE_H);
		position->position = sf::Vector2f(BUTTONS_PANEL_CENTER, level_size.y / 2.f);
		position->position.y += TITLE_H / 2;
	}

	// Level buttons
	auto AddLevelMenuButton = [BUTTONS_HEIGHT](ECSScene& level, std::string button_text, float width) {
		// Button
		auto [button_id, size] = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(width, BUTTONS_HEIGHT));
		level.AddComponent<MenuNavigable>(button_id);
		level.GetComponent<Shader>(button_id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
		// Text
		auto [text_id, _] = CreateScrollingText(level, sf::Vector2f(0, 0), button_text, 75u);

		EntitiesHandle main_button = { { button_id, text_id }, size };
		return main_button;
	};

	auto AddLevelMenuRow = [ui, AddLevelMenuButton, BUTTONS_WIDTH, BUTTONS_HEIGHT, is_in_level_editing = is_in_level_editing_](ECSScene& level, std::string button_text, std::function<void()> button_function) {
		float edit_buttons_width = 1 * float(BLOCK_SIZE);
		float edit_buttons_margin = 0.25 * float(BLOCK_SIZE);
		std::vector<EntitiesHandle> row_items;
		EntitiesHandle main_button = AddLevelMenuButton(level, button_text, is_in_level_editing ? BUTTONS_WIDTH - 2 * (edit_buttons_width + edit_buttons_margin) : BUTTONS_WIDTH);
		level.AddComponent<OnReleasedThisFrame>(std::get<std::vector<int>>(main_button)[0])->func = button_function;
		ui->button_entity_ids.push_back(std::get<std::vector<int>>(main_button)[0]);
		ui->text_entity_ids.push_back(std::get<std::vector<int>>(main_button)[1]);
		if (!is_in_level_editing)
		{
			return main_button;
		}
		EntityHandle edit_name_button = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(edit_buttons_width, BUTTONS_HEIGHT));
		EntityHandle delete_level_button = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(edit_buttons_width, BUTTONS_HEIGHT));
		level.AddComponent<StickyButton>(std::get<int>(edit_name_button));
		level.AddComponent<StickyButton>(std::get<int>(delete_level_button));
		level.GetComponent<Shader>(std::get<int>(edit_name_button))->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
		level.GetComponent<Shader>(std::get<int>(delete_level_button))->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";

		ui->edit_name_button_entity_ids.push_back(std::get<int>(edit_name_button));
		ui->delete_level_button_entity_ids.push_back(std::get<int>(delete_level_button));
		row_items.push_back(main_button);
		row_items.push_back(AdaptToEntitiesHandle(edit_name_button));
		row_items.push_back(AdaptToEntitiesHandle(delete_level_button));
		return HorizontalEntityLayout(level, sf::Vector2f(0, 0), row_items, edit_buttons_margin);
	};

	std::vector<EntitiesHandle> scroll_menu_items;
	for (const auto& level_id : level_groups.at(at_group))
	{
		EntitiesHandle row = AddLevelMenuRow(level, GetLevelDisplayNameFromId(level_id), std::bind(&LevelMenuSystem::EnterLevel, this, level_id));
		scroll_menu_items.push_back(row);
	}
	for (const auto& [button_id, level_id] : zip(ui->button_entity_ids, level_groups.at(at_group)))
	{
		if (ui->at_level_id == level_id)
		{
			level.GetComponent<MenuNavigator>(menu_navigator_id)->currently_at_entity_id = button_id;
			break;
		}
	}
	if (is_in_level_editing_)
	{
		EntitiesHandle new_level_button = AddLevelMenuButton(level, "+", BUTTONS_WIDTH);
		ui->new_level_button_id = std::get<std::vector<int>>(new_level_button)[0];
		scroll_menu_items.push_back(new_level_button);
	}
	auto [scroll_menu_item_ids, scroll_menu_items_size] = VerticalEntityLayout(level, sf::Vector2f(BUTTONS_PANEL_CENTER, TITLE_H), scroll_menu_items, BUTTONS_SPACING, StartEdge);
	scroll_window->entities = scroll_menu_item_ids;

	{ // Level preview
		sf::Vector2f position = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE / 2), level.GetSize().y * (LEVEL_PREVIEW_SCALE / 2));
		sf::Vector2f width_and_height = sf::Vector2f(level.GetSize().x * LEVEL_PREVIEW_SCALE, level.GetSize().y * LEVEL_PREVIEW_SCALE);
		auto [entity_id, size] = CreateTexturedRectangle(level, position, width_and_height, UI_BASE_DRAW_PRIORITY, "", true);
		ui->level_preview_id = entity_id;
		if (ui->at_level_id.has_value())
		{
			GenerateLevelPreview(level, ui, ui->at_level_id.value(), generate_level_texture_);
		}
	}

	{ // Stats display
		sf::Vector2f badge_center_positions = sf::Vector2f(level_size.x * (1 - LEVEL_PREVIEW_SCALE) + 3.75 * BLOCK_SIZE, level_size.y * (0.5 + 0.5 * LEVEL_PREVIEW_SCALE));
		std::vector<EntitiesHandle> entities_handles;
		for (int i = 0; i < 4; i++)
		{
			EntityHandle stats_badge = CreateStatsBadge(level, sf::Vector2f(0, 0), i, 50, "", false);
			level.GetComponent<FillColor>(std::get<int>(stats_badge))->color.a = 0;
			entities_handles.push_back(AdaptToEntitiesHandle(stats_badge));
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