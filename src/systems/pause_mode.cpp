#include "systems/pause_mode.hpp"
#include "entity_creation.hpp"
#include "utils/container_operations.hpp"
#include "utils/level_id.hpp"
#include "utils/string_manip.hpp"
#include < sstream>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>

const std::map<LevelState, float> PAUSE_MENU_DELAY { { COMPLETED, 2.f }, { PLAYING, 1.f }, { FAILED, 1.f } }; //seconds

void PauseMode::Give(
	const std::map<std::string, std::vector<std::string>>* level_groups,
	const std::map<int, std::map<std::string, float>>* level_completion_time_records,
	std::function<void(std::string)> set_level,
	std::function<void(bool)> set_edit_mode)
{
	level_groups_ = level_groups;
	level_completion_time_records_ = level_completion_time_records;
	set_level_ = set_level;
	set_edit_mode_ = set_edit_mode;
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
	assert(level_mode == PLAY_MODE || level_mode == PAUSE_MODE || level_mode == READY_MODE);

	MenuDelayTimer* menu_delay_timer = level.GetSingleton<MenuDelayTimer>();
	if (level_mode == PLAY_MODE && level_state != PLAYING)
	{
		menu_delay_timer->duration += dt;
	}

	bool lost_focus = !globals.render_window.hasFocus() && false; //for debugging
	if ((level_mode != PAUSE_MODE)
			&& cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape]
		|| (level_mode == PLAY_MODE
			&& (lost_focus || menu_delay_timer->duration > PAUSE_MENU_DELAY.at(level_state))))
	{
		level.SetMode(PAUSE_MODE);
		SetupPauseMenu(level);
		level.DeleteEntitiesWith<MenuDelayTimer>();
	}
	else if (level_mode != PAUSE_MODE)
	{
		level.DeleteEntitiesWith<PauseMenuItem>();
	}
}
void PauseMode::SetupPauseMenu(Level& level)
{
	LevelState level_state = level.ComputeState();
	std::vector<entities_handle> entities_handles;
	std::string menu_title;

	int background_blur_id = level.CreateEntityId();
	level.AddComponent<DrawInfo>(background_blur_id, { "content\\textures\\gray.png", false, 0 });
	level.AddComponent<WidthAndHeight>(background_blur_id)->width_and_height = level.GetSize();
	level.AddComponent<Position>(background_blur_id)->position = level.GetSize() / 2.f;
	level.AddComponent<FillColor>(background_blur_id)->color.a = 200;
	level.AddComponent<DrawPriority>(background_blur_id)->draw_priority = 50;
	level.AddComponent<PauseMenuItem>(background_blur_id);

	auto AddButton = [&](std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key) {
		entity_handle button_handle = CreateNavigatorButton(level, sf::Vector2f(0, 0), button_function, button_text, shortcut_key);
		entities_handles.push_back(AdaptToEntitiesHandle(button_handle));
	};

	if (in_level_creator_)
	{
		if (in_edit_mode_)
		{
			AddButton([&]() { level.SetMode(PLAY_MODE); }, "Continue editing", sf::Keyboard::Escape);
			AddButton([&]() { set_edit_mode_(false); level.SetMode(READY_MODE); }, "Test level", sf::Keyboard::Unknown);
		}
		else
		{
			if (level_state == PLAYING)
			{
				AddButton([&]() { level.SetMode(PLAY_MODE); }, "Continue", sf::Keyboard::Escape);
			}
			AddButton([&]() { set_edit_mode_(true); level.SetMode(PLAY_MODE); }, "Edit level", sf::Keyboard::Unknown);
			AddButton(std::bind(set_level_, active_level_id_), "Restart level", sf::Keyboard::R);
		}
	}
	else
	{
		if (level_state == PLAYING)
		{
			menu_title = "Paused";
			AddButton([&]() { level.SetMode(PLAY_MODE); }, "Continue", sf::Keyboard::Escape);
		}

		if (level_state == COMPLETED)
		{
			menu_title = "Level Complete";

			float duration = level.GetSingleton<LevelCompletionTimer>()->duration;
			int coin_count = level.GetSingleton<CoinCounter>()->coin_counter;
			bool is_new_record = duration == level_completion_time_records_->at(coin_count).at(active_level_id_);
			std::stringstream ss;

			entity_handle badge_handle = CreateStatsBadge(level, sf::Vector2f(0, 0), coin_count, 255, RightShiftString(FloatToStringWithPrecision(duration, 2), 13), is_new_record);
			entities_handles.push_back(AdaptToEntitiesHandle(badge_handle));

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
		AddButton(std::bind(set_level_, active_level_id_), "Restart level", sf::Keyboard::R);
	}

	AddButton(std::bind(set_level_, LEVEL_MENU), "Level menu", sf::Keyboard::Unknown);
	AddButton(std::bind(set_level_, MAIN_MENU), "Main menu", sf::Keyboard::Unknown);

	int navigator_id = level.AddBlueprint(BPMenuNavigator);
	level.AddComponent<PauseMenuItem>(navigator_id);

	entity_handle title_handle = CreateText(level, sf::Vector2f(0, 0), menu_title, unsigned(240));
	entities_handles.insert(entities_handles.begin(), AdaptToEntitiesHandle(title_handle));

	auto [ids, height] = VerticalEntityLayout(level, level.GetSize() / 2.f, entities_handles, BLOCK_SIZE);
	for (auto id : ids)
	{
		level.AddComponent<PauseMenuItem>(id);
	}
}
