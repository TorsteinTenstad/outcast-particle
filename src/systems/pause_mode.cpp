#include "systems/pause_mode.hpp"
#include "components/coin.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "components/timers.hpp"
#include "edit_mode_blueprint_menu_functions.hpp"
#include "entity_creation.hpp"
#include "utils/container_operations.hpp"
#include "utils/level_id.hpp"
#include "utils/string_manip.hpp"
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>

class PauseMenuItem
{};

const std::map<LevelState, float> PAUSE_MENU_DELAY { { COMPLETED, 2.f }, { PLAYING, 1.f }, { FAILED, 1.f } }; //seconds

void PauseMode::Give(
	std::function<Level&(std::string)> set_level,
	const std::map<std::string, std::vector<std::string>>* level_groups,
	const RecordsManager* records)
{
	set_level_ = set_level;
	level_groups_ = level_groups;
	records_ = records;
}
void PauseMode::Update(Level& level, float dt)
{
	if (IsMenu(active_level_id_))
	{
		return;
	}

	LevelState level_state = level.ComputeState();
	LevelMode level_mode = level.GetMode();

	assert(level_state == COMPLETED || level_state == PLAYING || level_state == FAILED); // To remind us to verify the state logic when adding new states
	assert(level_mode == PLAY_MODE || level_mode == PAUSE_MODE || level_mode == EDIT_MODE || level_mode == READY_MODE);

	MenuDelayTimer* menu_delay_timer = level.GetSingleton<MenuDelayTimer>();
	if (level_mode == PLAY_MODE && level_state != PLAYING)
	{
		menu_delay_timer->duration += dt;
	}

	bool lost_focus = !globals.render_window.hasFocus() && false; //for debugging
	if ((level_mode == PLAY_MODE || level_mode == EDIT_MODE)
			&& cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape]
		|| (level_mode == PLAY_MODE
			&& (lost_focus || menu_delay_timer->duration > PAUSE_MENU_DELAY.at(level_state))))
	{
		level.SetMode(PAUSE_MODE);
		SetupPauseMenu(level, level_mode);
		level.DeleteEntitiesWith<MenuDelayTimer>();
	}
	else if (level_mode != PAUSE_MODE)
	{
		level.DeleteEntitiesWith<PauseMenuItem>();
	}
}
void PauseMode::SetupPauseMenu(Level& level, LevelMode previous_mode)
{
	CloseBlueprintMenu(level);
	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponent<PauseMenuItem>(entity); });
	LevelState level_state = level.ComputeState();
	std::vector<EntitiesHandle> entities_handles;
	std::string menu_title;
	float scale = level.GetScale();

	CreateScreenWideBlur(level, level.GetSize(), UI_BASE_DRAW_PRIORITY - 1);

	auto AddButton = [&](std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key) {
		EntityHandle button_handle = CreateNavigatorButton(level, sf::Vector2f(0, 0), button_function, button_text, shortcut_key, sf::Vector2f(10, 2) * scale * float(BLOCK_SIZE));
		entities_handles.push_back(ToEntitiesHandle(button_handle));
	};

	if (previous_mode == PLAY_MODE)
	{
		if (level_state == PLAYING)
		{
			menu_title = "Paused";
			AddButton([&]() { level.SetMode(PLAY_MODE); }, "Continue", sf::Keyboard::Escape);
		}

		if (level_state == COMPLETED && !is_in_level_editing_)
		{
			menu_title = "Level Complete";

			float duration = level.GetSingleton<LevelCompletionTimer>()->duration;
			int coin_count = level.GetSingleton<CoinCounter>()->coin_counter;

			std::optional<float> record = records_->GetRecord(active_level_id_, coin_count);
			assert(record.has_value());
			bool is_new_record = duration == record.value_or(0);
			std::stringstream ss;

			EntityHandle badge_handle = CreateStatsBadge(level, sf::Vector2f(0, 0), coin_count, 255, LeftPad(CreateBadgeText(duration, 2 + globals.general_config.display_precise_badge_time), 16), is_new_record, level.GetScale());
			entities_handles.push_back(ToEntitiesHandle(badge_handle));

			auto level_group = level_groups_->at(GetGroupNameFromId(active_level_id_));
			auto active_level_index = std::find(level_group.begin(), level_group.end(), active_level_id_);
			auto next_level_index = ++active_level_index;
			if (next_level_index != level_group.end())
			{
				AddButton(std::bind(set_level_, *next_level_index), "Next level", sf::Keyboard::Unknown);
			}
		}

		if (level_state == FAILED)
		{
			menu_title = "You Died";
		}

		AddButton([&]() {level.SetMode(READY_MODE); level.LoadFromFile(); }, "Restart level", sf::Keyboard::R);

		if (is_in_level_editing_)
		{
			AddButton([&]() { level.SetMode(EDIT_MODE); }, "Edit level", sf::Keyboard::Unknown);
		}
	}

	if (previous_mode == EDIT_MODE)
	{
		AddButton([&]() { level.SetMode(EDIT_MODE); }, "Continue editing", sf::Keyboard::Escape);
		AddButton([&]() { level.SetMode(READY_MODE); }, "Test level", sf::Keyboard::Unknown);
	}

	if (is_in_level_editing_ && !level.editor.IsEmpty())
	{
		std::function<void(void)> return_to_level_menu_func = [&level, set_level_ = this->set_level_]() {
			CreateBlockingPopupMenu(std::ref(level), level.GetSize(), "Save changes?", { { "Save", [&]() { set_level_(LEVEL_MENU); }, sf::Keyboard::Unknown }, { "Discard", [&]() { level.editor.UndoAll(); level.SaveToFile(); set_level_(LEVEL_MENU); }, sf::Keyboard::Unknown }, { "Cancel", [&]() {}, sf::Keyboard::Escape } }, {}, level.GetScale());
		};

		std::function<void(void)> return_to_main_menu_func = [&level, set_level_ = this->set_level_]() {
			CreateBlockingPopupMenu(std::ref(level), level.GetSize(), "Save changes?", { { "Save", [&]() { set_level_(MAIN_MENU); }, sf::Keyboard::Unknown }, { "Discard", [&]() { level.editor.UndoAll(); level.SaveToFile(); set_level_(MAIN_MENU); }, sf::Keyboard::Unknown }, { "Cancel", [&]() {}, sf::Keyboard::Escape } }, {}, level.GetScale());
		};

		AddButton(return_to_level_menu_func, "Level Menu", sf::Keyboard::Unknown);
		AddButton(return_to_main_menu_func, "Main menu", sf::Keyboard::Unknown);
	}
	else
	{
		AddButton([&]() { set_level_(LEVEL_MENU); }, "Level menu", sf::Keyboard::Unknown);
		AddButton([&]() { set_level_(MAIN_MENU); }, "Main menu", sf::Keyboard::Unknown);
	}

	EntityHandle title_handle = CreateText(level, sf::Vector2f(0, 0), menu_title, unsigned(240) * scale);
	entities_handles.insert(entities_handles.begin(), ToEntitiesHandle(title_handle));

	auto [entities, height] = VerticalEntityLayout(level, level.GetSize() / 2.f, entities_handles, BLOCK_SIZE * scale);
	auto [navigator_id, navigator_size] = CreateMenuNavigator(level);
}
