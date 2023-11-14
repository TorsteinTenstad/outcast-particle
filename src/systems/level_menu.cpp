#pragma once
#include "components/level_menu.hpp"
#include "Components/sound_info.hpp"
#include "Components/tooltip.hpp"
#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/menu_navigator.hpp"
#include "components/open_file_dialog.hpp"
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
#include "platform_specific/platform_specific.hpp"
#include "systems/level_menu.hpp"
#include "utils/container_operations.hpp"
#include "utils/level_id.hpp"
#include "utils/string_manip.hpp"
#include <cassert>
#include <iomanip>
#include <sstream>
#include <vector>

#define LEVEL_PREVIEW_SCALE 0.6

class LeaderboardEntity
{};

static void GenerateLevelPreview(Level& level, LevelMenuUI* ui,
	std::string level_id,
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture)
{
	unsigned texture_size_x = unsigned(level.GetSize().x * LEVEL_PREVIEW_SCALE);
	unsigned texture_size_y = unsigned(level.GetSize().y * LEVEL_PREVIEW_SCALE);
	std::string texture_identifier = generate_level_texture(level_id, texture_size_x, texture_size_y);
	level.GetComponent<DrawInfo>(ui->level_preview_entity)->image_path = texture_identifier;
}

static void UpdateStatsBadges(Level& level, LevelMenuUI* ui,
	const RecordsManager* records, const ServerTransceiver* server_transceiver)
{
	if (!ui->at_level_id.has_value())
	{
		level.DeleteEntitiesWith<LeaderboardEntity>();
		return;
	}
	std::string at_level_id = ui->at_level_id.value();
	globals.general_config.active_badge_button_id = -1;
	for (int i = 0; i < 4; i++)
	{
		if (level.HasComponents<StickyButtonDown>(ui->badge_entities[i]))
		{
			globals.general_config.active_badge_button_id = i;
			level.GetComponent<Position>(ui->connector_entity)->position.y = level.GetComponent<Position>(ui->badge_entities[i])->position.y;
		}
	}
	for (auto [entity, fill_color, leaderboard_entity] : level.GetEntitiesWith<FillColor, LeaderboardEntity>())
	{
		fill_color->color.a = 255 * (globals.general_config.active_badge_button_id != -1);
	}

	std::vector<LeaderboardEntryDisplayInfo> leaderboard_display_info = server_transceiver->GetLeaderboardDisplayInfo(at_level_id, globals.general_config.active_badge_button_id);

	int i = 0;
	auto size = level.GetSize();
	sf::Vector2f record_center_positions = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE) + 12.6 * BLOCK_SIZE, level.GetSize().y * (0.5 + 0.5 * LEVEL_PREVIEW_SCALE));
	std::vector<EntitiesHandle> records_layout;
	int triple_dots_index = ui->record_block_entities.size();
	int prev_rank = 0;
	std::string missing_record_text = "";
	if (leaderboard_display_info.size() == 0)
	{
		if (server_transceiver->has_received_leaderboard)
		{
			missing_record_text = "No scores yet";
		}
		else
		{
			missing_record_text = "    Could not fetch\nleaderboard from server";
			std::optional<float> local_record = records->GetRecord(at_level_id, globals.general_config.active_badge_button_id);
			if (local_record.has_value())
			{
				leaderboard_display_info.push_back(LeaderboardEntryDisplayInfo { -1, globals.steam_username.value_or("You"), local_record.value() });
			}
		}
	}
	level.GetComponent<Text>(ui->backdrop_entity)->content = missing_record_text;
	for (auto entity : ui->record_block_entities)
	{
		level.GetComponent<Text>(entity)->outline_thickness = 0;
		std::string text = "";
		if (i < leaderboard_display_info.size())
		{
			LeaderboardEntryDisplayInfo info = leaderboard_display_info[i];
			if (info.steam_username == globals.steam_username.value_or("You"))
			{
				level.GetComponent<Text>(entity)->outline_color = sf::Color(80, 80, 80, 255);
				level.GetComponent<Text>(entity)->outline_thickness = 5;
			}
			std::string username_formatted = info.steam_username;
			int username_format_length = 20;
			if (username_formatted.size() > username_format_length)
			{
				username_formatted = username_formatted.substr(0, username_format_length - 3) + "...";
			}
			username_formatted = RightPad(username_formatted, username_format_length);
			std::string rank_string = info.rank > 0 ? std::to_string(info.rank) : "?";
			text = "#" + rank_string + "   " + username_formatted + " " + LeftPad(CreateBadgeText(info.time, 2 + globals.general_config.display_precise_badge_time), 7) + "s";
			if (info.rank - prev_rank > 1) { triple_dots_index = std::min(triple_dots_index, i); }
			prev_rank = info.rank;
		}
		else
		{
			level.GetComponent<FillColor>(ui->triple_dot_entity)->color.a = 0;
			level.GetComponent<FillColor>(entity)->color.a = 0;
		}
		records_layout.push_back(ToEntitiesHandle({ entity, level.GetComponent<WidthAndHeight>(entity)->width_and_height }));
		level.GetComponent<Position>(entity)->position = sf::Vector2f(0, 0);
		level.GetComponent<Text>(entity)->content = text;
		i++;
	}
	records_layout.insert(records_layout.begin() + triple_dots_index, ToEntitiesHandle({ ui->triple_dot_entity, level.GetComponent<WidthAndHeight>(ui->triple_dot_entity)->width_and_height }));
	level.GetComponent<Position>(ui->triple_dot_entity)->position = sf::Vector2f(0, 0);
	auto entities = VerticalEntityLayout(level, record_center_positions, records_layout, BLOCK_SIZE / 4);
}

static void UpdateLevelPreview(Level& level, LevelMenuUI* ui, const std::map<std::string, std::vector<std::string>>& level_groups, std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture, const RecordsManager* records_)
{
	assert(ui->at_group.has_value());
	std::string at_group = ui->at_group.value();
	for (const auto& [button_entity, level_id] : zip(ui->button_entities, level_groups.at(at_group)))
	{
		if (!level.HasComponents<HoveredStartedThisFrame>(button_entity)) { continue; }
		if (ui->displaying_level_id.has_value() && ui->displaying_level_id.value() == level_id) { continue; }

		ui->displaying_level_id = level_id;
		ui->at_level_id = level_id;
		LevelMenuUI::last_at_level_id[at_group] = level_id;
		LevelMenuUI::last_at_level_group = at_group;
		GenerateLevelPreview(level, ui, level_id, generate_level_texture);
		return;
	}
	if (ui->new_level_button_id.has_value()
		&& level.HasComponents<HoveredStartedThisFrame>(ui->new_level_button_id.value()))
	{
		GenerateLevelPreview(level, ui, NEW_LEVEL_TEMPLATE_FILE.string(), generate_level_texture);
		ui->displaying_level_id = NEW_LEVEL_TEMPLATE_FILE.string();
	}
}

static void RequestRedraw(Level& level, LevelMenuUI* ui, std::string level_group, std::optional<std::string> at_level_id = std::nullopt)
{
	auto [entity, request] = level.CreateEntityWith<ReDrawLevelMenuRequest>();
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
	const RecordsManager* records,
	const ServerTransceiver* server_transceiver,
	std::function<Level&(std::string)> set_level,
	std::function<Level&(std::string)> set_level_and_edit,
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture)
{
	level_manager_ = level_manager;
	records_ = records;
	server_transceiver_ = server_transceiver;
	set_level_ = set_level;
	set_level_and_edit_ = set_level_and_edit;
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
		auto [entity, ui] = level.CreateEntityWith<LevelMenuUI>();
		ui->at_group = at_group;
		ui->at_level_id = at_level_id;
		SetupUI(level, ui);
	}

	if (LevelMenuUI* ui = level.FindSingleton<LevelMenuUI>())
	{
		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::F3])
		{
			assert(ui->at_group.has_value());
			globals.content_access_options.ToggleAllLevelsAreEditable();
			RequestRedraw(level, ui, ui->at_group.value(), ui->at_level_id);
		}
		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::F4])
		{
			assert(ui->at_group.has_value());
			globals.content_access_options.ToggleTrialBuild();
			RequestRedraw(level, ui, ui->at_group.value(), ui->at_level_id);
		}
		UpdateUI(level, ui);
	}
}

void LevelMenuSystem::UpdateUI(Level& level, LevelMenuUI* ui)
{
	const std::map<std::string, std::vector<std::string>>& level_groups = level_manager_->GetLevels();
	assert(ui->at_group.has_value());
	std::string at_group = ui->at_group.value();

	UpdateLevelPreview(level, ui, level_manager_->GetLevels(), generate_level_texture_, records_);

	UpdateStatsBadges(level, ui, records_, server_transceiver_);

	if (level.HasComponents<ReleasedThisFrame>(ui->next_group_button_entity))
	{
		std::string new_group = NextInMap(level_groups, at_group)->first;
		return RequestRedrawIfLevelGroupIsNew(level, ui, new_group);
	}
	if (level.HasComponents<ReleasedThisFrame>(ui->prev_group_button_entity))
	{
		std::string new_group = PrevInMap(level_groups, at_group)->first;
		return RequestRedrawIfLevelGroupIsNew(level, ui, new_group);
	}
	if (level.HasComponents<ReleasedThisFrame>(ui->dot_indicator_entity))
	{
		float pos_x = level.GetComponent<Position>(ui->dot_indicator_entity)->position.x;
		float w = level.GetComponent<WidthAndHeight>(ui->dot_indicator_entity)->width_and_height.x;
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
		if (!globals.content_access_options.IsLevelGroupEditable(at_group))
		{
			CreateBlockingInformationMenu(level, level.GetSize(), "Creating custom levels requires full version");
			return;
		}
		std::string new_level_id = level_manager_->CreateNewLevel(at_group);
		return RequestRedraw(level, ui, at_group, new_level_id);
	}
	if (ui->import_level_button_id.has_value() && level.HasComponents<ReleasedThisFrame>(ui->import_level_button_id.value()))
	{
		std::optional<std::string> path = GetPathFromOpenFileDialog();
		if (path.has_value())
		{
			std::optional<std::string> new_level_id = level_manager_->ImportLevel(path.value(), at_group);
			if (new_level_id.has_value())
			{
				return RequestRedraw(level, ui, at_group, new_level_id);
			}
		}
	}

	if (ui->delete_level_button_entities.size() > 0)
	{
		for (const auto& [main_button_id, text_id, rename_level_button_id, edit_level_button_id, delete_button_id, level_id] : zip(
				 ui->button_entities,
				 ui->text_entities,
				 ui->rename_level_button_entities,
				 ui->edit_level_button_entities,
				 ui->delete_level_button_entities,
				 level_groups.at(at_group)))
		{
			if (level.HasComponents<HoveredStartedThisFrame>(rename_level_button_id)
				|| level.HasComponents<HoveredStartedThisFrame>(edit_level_button_id)
				|| level.HasComponents<HoveredStartedThisFrame>(delete_button_id))
			{
				level.GetComponent<MenuNavigator>(ui->menu_navigator_entity)->current_snap_position = SnapPosition(level, main_button_id);
			}
			if (level.HasComponents<ReleasedThisFrame>(delete_button_id))
			{
				std::function<void(void)> delete_func = [this, &level, ui, at_group, level_id = level_id]() {
					level_manager_->DeleteLevel(level_id);
					RequestRedraw(level, ui, at_group);
				};
				CreateConfirmMenu(level, level.GetSize(), "Delete level?", delete_func);
				return;
			}
			if (level.HasComponents<ReleasedThisFrame>(rename_level_button_id))
			{
				EntityHandle rename_box = CreateText(level, sf::Vector2f(0, 0), GetLevelDisplayNameFromId(level_id));
				Entity rename_box_id = GetEntity(rename_box);
				level.AddComponent<TextBox>(rename_box_id);
				auto rename_func = [&level, level_manager_ = level_manager_, level_id = level_id, text_id = text_id, ui, rename_box_id]() {
					std::string new_display_name = level.GetComponent<Text>(rename_box_id)->content;
					std::string new_level_id = level_manager_->RenameLevel(level_id, new_display_name);
					level.GetComponent<Text>(text_id)->content = new_display_name;
					RequestRedraw(level, ui, GetGroupNameFromId(new_level_id), new_level_id);
				};
				CreateBlockingPopupMenu(level, level.GetSize(), "Rename level", { { "Rename", rename_func, sf::Keyboard::Unknown }, { "Cancel", []() {}, sf::Keyboard::Escape } }, ToEntitiesHandle((rename_box)));
			}
			else if (level.HasComponents<ReleasedThisFrame>(edit_level_button_id))
			{
				CreateConfirmMenu(level, level.GetSize(), "Edit level?", std::bind(set_level_and_edit_, level_id));
			}
		}
	}
}

void LevelMenuSystem::SetupUI(Level& level, LevelMenuUI* ui)
{
	assert(ui->button_entities.size() == 0);

	const std::map<std::string, std::vector<std::string>>& level_groups = level_manager_->GetLevels();
	if (!ui->at_group.has_value())
	{
		ui->at_group = LevelMenuUI::last_at_level_group.value_or(level_groups.begin()->first);
	}
	std::string at_group = ui->at_group.value();
	bool levels_are_editable = globals.content_access_options.IsLevelGroupEditable(at_group);

	if (!ui->at_level_id.has_value())
	{
		std::vector<std::string> levels_in_group = level_groups.at(at_group);
		auto it = LevelMenuUI::last_at_level_id.find(at_group);
		if (it != LevelMenuUI::last_at_level_id.end())
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
		auto [entity, shader, receives_button_events, draw_info, draw_priority, width_and_height, position] = level.CreateEntityWith<Shader, ReceivesButtonEvents, DrawInfo, DrawPriority, WidthAndHeight, Position>();
		ui->dot_indicator_entity = entity;
		shader->fragment_shader_path = (SHADERS_DIR / "dots_indicator.frag").string();
		shader->int_uniforms["n_dots"] = level_groups.size();
		shader->int_uniforms["active_dot"] = std::distance(level_groups.cbegin(), level_groups.find(at_group));
		position->position.x = BUTTONS_PANEL_CENTER;
		position->position.y = TITLE_H - dot_indicator_h / 2 - 0.5 * float(BLOCK_SIZE);
		width_and_height->width_and_height.x = 400;
		width_and_height->width_and_height.y = dot_indicator_h;
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
	}
	{ // Title
		auto [entity, text, draw_priority, position] = level.CreateEntityWith<Text, DrawPriority, Position>();
		draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
		text->size = 100;
		text->content = GetGroupDisplayNameFromGroupName(at_group);
		position->position = sf::Vector2f(BUTTONS_PANEL_CENTER, TITLE_H / 2 - 0.5 * dot_indicator_h);
	}

	// Group navigation buttons
	for (int p : { -1, 1 })
	{
		sf::Vector2f pos = sf::Vector2f(BUTTONS_PANEL_CENTER + p * (4) * float(BLOCK_SIZE), TITLE_H / 2);
		sf::Vector2f w_h = sf::Vector2f(2, 2) * float(BLOCK_SIZE);
		{
			auto [entity, draw_info, shader, fill_color, mouse_interaction_dependent_fill_color, draw_priority, width_and_height, position, receives_mouse_events, shortcut_key, sound_info, can_disable_button, tooltip] =
				level.CreateEntityWith<DrawInfo, Shader, FillColor, MouseInteractionDependentFillColor, DrawPriority, WidthAndHeight, Position, ReceivesButtonEvents, ShortcutKey, SoundInfo, CanDisableButton, Tooltip>();
			if (p == -1)
			{
				ui->prev_group_button_entity = entity;
				shortcut_key->key = sf::Keyboard::Key::Left;
				tooltip->text = "Go to previous level group [Left arrow]";
			}
			if (p == 1)
			{
				ui->next_group_button_entity = entity;
				shortcut_key->key = sf::Keyboard::Key::Right;
				tooltip->text = "Go to next level group [Right arrow]";
			}
			can_disable_button->func = [is_end = (p == -1) ? PrevInMap(level_groups, at_group)->first == at_group : NextInMap(level_groups, at_group)->first == at_group]() { return !is_end; };
			position->position = pos;
			width_and_height->width_and_height = w_h;
			draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY;
			shader->fragment_shader_path = (SHADERS_DIR / "round_corners.frag").string();
			sound_info->sound_paths = { { ON_CLICK, (SOUNDS_DIR / "click.wav").string() } };
		}

		{
			auto [entity, draw_info, draw_priority, width_and_height, position, can_disable_button, fill_color] =
				level.CreateEntityWith<DrawInfo, DrawPriority, WidthAndHeight, Position, CanDisableButton, FillColor>();
			can_disable_button->func = [is_end = (p == -1) ? PrevInMap(level_groups, at_group)->first == at_group : NextInMap(level_groups, at_group)->first == at_group]() { return !is_end; };
			can_disable_button->regain_button_events = false;
			draw_info->image_path = (p == -1) ? (TEXTURES_DIR / "prev.png").string() : (TEXTURES_DIR / "next.png").string();
			position->position = pos;
			width_and_height->width_and_height = w_h;
			draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY + 1;
		}
	}
	// Scroll window
	auto [menu_navigator_entity, _] = CreateMenuNavigator(level);
	ui->menu_navigator_entity = menu_navigator_entity;
	ScrollableEntityContainer* scroll_window;
	{
		auto [entity, scroll_window_local, width_and_height, position] = level.CreateEntityWith<ScrollableEntityContainer, WidthAndHeight, Position>();
		scroll_window = scroll_window_local;
		scroll_window->entity_height = BUTTONS_HEIGHT;
		scroll_window->menu_navigator = menu_navigator_entity;
		width_and_height->width_and_height = sf::Vector2f(BUTTONS_WIDTH, level_size.y - TITLE_H);
		position->position = sf::Vector2f(BUTTONS_PANEL_CENTER, level_size.y / 2.f);
		position->position.y += TITLE_H / 2;
	}

	// Level buttons
	auto AddLevelMenuButton = [this, BUTTONS_HEIGHT, levels_are_editable](ECSScene& level, std::string level_id, float width) {
		// Button
		auto [button_id, size] = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(width, BUTTONS_HEIGHT));
		level.AddComponent<MenuNavigable>(button_id);
		Shader* shader = level.GetComponent<Shader>(button_id);
		shader->fragment_shader_path = (SHADERS_DIR / "level_button.frag").string();
		shader->int_uniforms["n_collected"] = records_->GetCoinsCollected(level_id);
		shader->float_uniforms["coin_alpha_mask"] = records_->GetRecord(level_id).has_value() && !levels_are_editable ? 1.0 : 0.0;
		// Text
		auto [text_id, _] = CreateScrollingText(level, sf::Vector2f(BLOCK_SIZE / 2 - width / 2, 0), GetLevelDisplayNameFromId(level_id), 75u);
		level.GetComponent<Text>(text_id)->origin = TextOrigin::REFERENCE_HEIGHT_LEFT;

		EntitiesHandle main_button = { { button_id, text_id }, size };
		return main_button;
	};

	auto AddLevelMenuRow = [ui, AddLevelMenuButton, BUTTONS_WIDTH, BUTTONS_HEIGHT, levels_are_editable](ECSScene& level, std::string level_id, std::function<void()> button_function) {
		float edit_buttons_width = 1 * float(BLOCK_SIZE);
		float edit_buttons_margin = 0.25 * float(BLOCK_SIZE);
		std::vector<EntitiesHandle> row_items;
		EntitiesHandle main_button = AddLevelMenuButton(level, level_id, levels_are_editable ? BUTTONS_WIDTH - 3 * (edit_buttons_width + edit_buttons_margin) : BUTTONS_WIDTH);
		level.AddComponent<OnReleasedThisFrame>(std::get<std::vector<Entity>>(main_button)[0])->func = button_function;
		ui->button_entities.push_back(std::get<std::vector<Entity>>(main_button)[0]);
		ui->text_entities.push_back(std::get<std::vector<Entity>>(main_button)[1]);
		if (!levels_are_editable)
		{
			return main_button;
		}
		row_items.push_back(main_button);
		const std::vector<std::vector<Entity>*> ui_button_containers = { &ui->rename_level_button_entities, &ui->edit_level_button_entities, &ui->delete_level_button_entities };
		const std::vector<std::string> icon_paths = { (TEXTURES_DIR / "edit.png").string(), (TEXTURES_DIR / "brush.png").string(), (TEXTURES_DIR / "delete.png").string() };
		const std::vector<std::string> tooltips = { "Rename level", "Edit level", "Delete level" };
		for (const auto& [ui_container, icon_path, tooltip] : zip(ui_button_containers, icon_paths, tooltips))
		{
			EntityHandle button_handle = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(edit_buttons_width, BUTTONS_HEIGHT));
			EntityHandle icon_handle = CreateTexturedRectangle(level, sf::Vector2f(0, 0), sf::Vector2f(edit_buttons_width, BUTTONS_HEIGHT), UI_BASE_DRAW_PRIORITY + 1, icon_path, false);
			Entity button_id = GetEntity(button_handle);
			level.GetComponent<Shader>(button_id)->fragment_shader_path = (SHADERS_DIR / "scroll_and_round_corners.frag").string();
			level.AddComponent<Shader>(GetEntity(icon_handle))->fragment_shader_path = (SHADERS_DIR / "scroll_and_round_corners.frag").string();
			level.AddComponent<Tooltip>(button_id)->text = tooltip;
			row_items.push_back(ToEntitiesHandle(button_handle, icon_handle));
			ui_container->push_back(button_id);
		}
		return HorizontalEntityLayout(level, sf::Vector2f(0, 0), row_items, edit_buttons_margin);
	};

	std::vector<EntitiesHandle> scroll_menu_items;
	for (const auto& level_id : level_groups.at(at_group))
	{
		std::string group_display_name = GetGroupDisplayNameFromId(level_id);
		std::function<void(void)> enter_level = [level_id, set_level = set_level_]() { set_level(level_id); };
		std::function<void(void)> show_level_inaccessible = [&level, group_display_name]() { CreateBlockingInformationMenu(level, level.GetSize(), "Levels from " + group_display_name + " requires full version"); };
		EntitiesHandle row = AddLevelMenuRow(level, level_id, globals.content_access_options.IsLevelAccessible(level_id) ? enter_level : show_level_inaccessible);
		scroll_menu_items.push_back(row);
	}
	for (const auto& [button_id, level_id] : zip(ui->button_entities, level_groups.at(at_group)))
	{
		if (ui->at_level_id == level_id)
		{
			level.GetComponent<MenuNavigator>(menu_navigator_entity)->current_snap_position = SnapPosition(level, button_id);
			break;
		}
	}
	if (levels_are_editable)
	{
		{
			EntitiesHandle handle = AddLevelMenuButton(level, "Create a new level", BUTTONS_WIDTH);
			Entity entity = std::get<std::vector<Entity>>(handle)[0];
			ui->new_level_button_id = entity;
			scroll_menu_items.push_back(handle);
		}
		{
			EntitiesHandle handle = AddLevelMenuButton(level, "Import level", BUTTONS_WIDTH);
			Entity entity = std::get<std::vector<Entity>>(handle)[0];
			ui->import_level_button_id = entity;
			scroll_menu_items.push_back(handle);
		}
	}
	auto [scroll_menu_item_entities, scroll_menu_items_size] = VerticalEntityLayout(level, sf::Vector2f(BUTTONS_PANEL_CENTER, TITLE_H), scroll_menu_items, BUTTONS_SPACING, StartEdge);
	scroll_window->entities = scroll_menu_item_entities;

	{ // Level preview
		sf::Vector2f position = sf::Vector2f(level.GetSize().x * (1 - LEVEL_PREVIEW_SCALE / 2), level.GetSize().y * (LEVEL_PREVIEW_SCALE / 2));
		sf::Vector2f width_and_height = sf::Vector2f(level.GetSize().x * LEVEL_PREVIEW_SCALE, level.GetSize().y * LEVEL_PREVIEW_SCALE);
		auto [entity, size] = CreateTexturedRectangle(level, position, width_and_height, UI_BASE_DRAW_PRIORITY, "", true);
		ui->level_preview_entity = entity;
		if (ui->at_level_id.has_value())
		{
			GenerateLevelPreview(level, ui, ui->at_level_id.value(), generate_level_texture_);
		}
		else if (ui->new_level_button_id.has_value())
		{
			GenerateLevelPreview(level, ui, NEW_LEVEL_TEMPLATE_FILE.string(), generate_level_texture_);
			ui->displaying_level_id = NEW_LEVEL_TEMPLATE_FILE.string();
		}
	}

	{ // Stats display
		sf::Vector2f badge_center_positions = sf::Vector2f(level_size.x * (1 - LEVEL_PREVIEW_SCALE) + 3 * BLOCK_SIZE, level_size.y * (0.5 + 0.5 * LEVEL_PREVIEW_SCALE));
		std::vector<EntitiesHandle> badge_entities_handles;
		std::vector<EntitiesHandle> record_entities_handles;
		std::vector<Entity> badge_entities;
		std::vector<Entity> record_entities;
		auto [backdrop_entity, backdrop_size] = CreateButtonTemplate(level, sf::Vector2f(badge_center_positions.x + 9.6 * BLOCK_SIZE, badge_center_positions.y), sf::Vector2f(12.7 * BLOCK_SIZE, 6.75 * BLOCK_SIZE));
		auto [connector_entity, connector_size] = CreateButtonTemplate(level, sf::Vector2f(level_size.x * float(1 - LEVEL_PREVIEW_SCALE / 2), 0), sf::Vector2f(10 * BLOCK_SIZE, 1.5 * BLOCK_SIZE - 1));
		{
			Text* text = level.AddComponent<Text>(backdrop_entity);
			ui->backdrop_entity = backdrop_entity;
		}
		for (Entity entity : { backdrop_entity, connector_entity })
		{
			level.AddComponent<LeaderboardEntity>(entity);
			level.GetComponent<FillColor>(entity)->color = PRESSED_COLOR;
			level.GetComponent<DrawPriority>(entity)->draw_priority = UI_BASE_DRAW_PRIORITY + 1;
		}
		ui->connector_entity = connector_entity;
		level.RemoveComponents<Shader>(connector_entity);
		auto [triple_dot_entity, triple_dot_size] = CreateTexturedRectangle(level, sf::Vector2f(9.6, 0) * float(BLOCK_SIZE), sf::Vector2f(1, 1) * 0.5f * BLOCK_SIZE, UI_BASE_DRAW_PRIORITY + 2, (TEXTURES_DIR / "more_vert.png").string(), false);
		std::get<FillColor*>(level.AddComponents<LeaderboardEntity, FillColor>(triple_dot_entity))->color = DEFAULT_COLOR; // Sacc readability to cut one line

		for (int i = 0; i < 4; i++)
		{
			auto [badge_entity, size] = CreateMouseEventButton(level, sf::Vector2f(0, 0), sf::Vector2f(6, 1.5) * float(BLOCK_SIZE) * level.GetScale());
			level.GetComponent<Shader>(badge_entity)->fragment_shader_path = (SHADERS_DIR / "stats_badge.frag").string();
			level.GetComponent<Shader>(badge_entity)->int_uniforms["n_collected"] = i;
			level.AddComponent<StickyButton>(badge_entity)->channel = 3;
			auto [icon_entity, icon_size] = CreateTexturedRectangle(level, sf::Vector2f(2, 0) * float(BLOCK_SIZE), sf::Vector2f(1.35, 1.35) * BLOCK_SIZE, UI_BASE_DRAW_PRIORITY + 2, (TEXTURES_DIR / "leaderboard.png").string(), false);
			auto [record_entity, record_size] = CreateButtonTemplate(level, sf::Vector2f(9.6, 0) * float(BLOCK_SIZE), sf::Vector2f(11.7, 1) * BLOCK_SIZE);
			level.AddComponent<Text>(record_entity)->size = 75u;
			level.AddComponent<LeaderboardEntity>(record_entity);
			level.GetComponent<FillColor>(record_entity)->color = DEFAULT_COLOR;
			level.GetComponent<DrawPriority>(record_entity)->draw_priority = UI_BASE_DRAW_PRIORITY + 2;
			badge_entities_handles.push_back({ { badge_entity, icon_entity }, size });
			record_entities_handles.push_back(ToEntitiesHandle({ record_entity, record_size }));
			badge_entities.push_back(badge_entity);
			record_entities.push_back(record_entity);
		}
		int active_badge_button_id = globals.general_config.active_badge_button_id;
		if (active_badge_button_id != -1)
		{
			level.AddComponent<StickyButtonDown>(badge_entities[active_badge_button_id]);
		}
		auto [entities, heights] = VerticalEntityLayout(level, badge_center_positions, badge_entities_handles, BLOCK_SIZE / 4);
		record_entities_handles.push_back(ToEntitiesHandle({ triple_dot_entity, triple_dot_size }));
		auto record_handles = VerticalEntityLayout(level, badge_center_positions, record_entities_handles, BLOCK_SIZE / 4);
		ui->badge_entities = badge_entities;
		ui->record_block_entities = record_entities;
		ui->triple_dot_entity = triple_dot_entity;
	}
}