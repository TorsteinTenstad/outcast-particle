#include "systems/pause_mode.hpp"
#include "entity_creation.hpp"
#include "utils/level_id.hpp"
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>

const std::map<LevelState, float> PAUSE_MENU_DELAY { { COMPLETED, 2.f }, { PLAYING, 1.f }, { FAILED, 1.f } }; //seconds

void PauseMode::Give(std::function<void(std::string)> set_level, const std::map<std::string, std::vector<std::string>>* level_groups)
{
	set_level_ = set_level;
	level_groups_ = level_groups;
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
	LevelState level_state = level.ComputeState();
	std::vector<std::function<void(void)>> button_functions;
	std::vector<std::function<std::tuple<std::vector<int>, float>(sf::Vector2f)>> entity_creating_functions;
	std::vector<sf::Keyboard::Key> shortcut_keys;
	std::vector<std::string> text;
	std::string menu_title;

	int background_blur_id = level.CreateEntityId();
	level.AddComponent<DrawInfo>(background_blur_id, { "content\\textures\\gray.png", false, 0 });
	level.AddComponent<WidthAndHeight>(background_blur_id)->width_and_height = level.GetSize();
	level.AddComponent<Position>(background_blur_id)->position = level.GetSize() / 2.f;
	level.AddComponent<FillColor>(background_blur_id)->color.a = 200;
	level.AddComponent<DrawPriority>(background_blur_id)->draw_priority = 50;
	level.AddComponent<PauseMenuItem>(background_blur_id);

	if (previous_mode == PLAY_MODE)
	{
		if (level_state == PLAYING)
		{
			menu_title = "Paused";
			text.push_back("Continue");
			button_functions.push_back([&]() { level.SetMode(PLAY_MODE); });
			shortcut_keys.push_back(sf::Keyboard::Escape);
		}

		if (level_state == COMPLETED && !is_in_level_editing_)
		{
			menu_title = "Level Complete";
			std::function<std::tuple<std::vector<int>, float>(sf::Vector2f)> create_badge_function = std::bind(&AddStatsBadge, std::ref(level), std::placeholders::_1, level.GetSingleton<CoinCounter>()->coin_counter);
			entity_creating_functions.push_back(create_badge_function);
			auto level_group = level_groups_->at(GetGroupNameFromId(active_level_id_));
			auto active_level_index = std::find(level_group.begin(), level_group.end(), active_level_id_);
			auto next_level_index = ++active_level_index;
			if (next_level_index != level_group.end())
			{
				text.push_back("Next level");
				button_functions.push_back(std::bind(set_level_, *next_level_index));
				shortcut_keys.push_back(sf::Keyboard::Unknown);
			}
		}

		if (level_state == FAILED)
		{
			menu_title = "You Died";
		}

		text.push_back("Restart level");
		button_functions.push_back(std::bind(set_level_, active_level_id_));
		shortcut_keys.push_back(sf::Keyboard::R);

		if (is_in_level_editing_)
		{
			text.push_back("Edit level");
			button_functions.push_back([&]() { level.SetMode(EDIT_MODE); });
			shortcut_keys.push_back(sf::Keyboard::Unknown);
		}
	}

	if (previous_mode == EDIT_MODE)
	{
		text.push_back("Continue editing");
		button_functions.push_back([&]() { level.SetMode(EDIT_MODE); });
		shortcut_keys.push_back(sf::Keyboard::Escape);

		text.push_back("Play level");
		button_functions.push_back([&]() { level.SetMode(READY_MODE); });
		shortcut_keys.push_back(sf::Keyboard::Unknown);
	}

	text.push_back("Level menu");
	button_functions.push_back(std::bind(set_level_, LEVEL_MENU));
	shortcut_keys.push_back(sf::Keyboard::Unknown);

	text.push_back("Main menu");
	button_functions.push_back(std::bind(set_level_, MAIN_MENU));
	shortcut_keys.push_back(sf::Keyboard::Unknown);

	std::function<std::tuple<std::vector<int>, float>(sf::Vector2f)> button_list_func = std::bind(&AddButtonList, std::ref(level), std::placeholders::_1, button_functions, text, shortcut_keys, level.GetScale(), level.GetScale() * 0.8, CenterCenter);
	std::function<std::tuple<std::vector<int>, float>(sf::Vector2f)> title_func = std::bind(&AddText, std::ref(level), std::placeholders::_1, menu_title, unsigned(240));
	entity_creating_functions.insert(entity_creating_functions.begin(), button_list_func);
	entity_creating_functions.push_back(title_func);
	AddMenuEntities(level, entity_creating_functions);
}

void PauseMode::AddMenuEntities(Level& level, std::vector<std::function<std::tuple<std::vector<int>, float>(sf::Vector2f)>> entity_creating_functions)
{
	float x_pos = level.GetSize().x / 2.f;
	float y_pos = level.GetSize().y / 2.f;
	std::vector<int> pause_menu_ids;
	for (auto function : entity_creating_functions)
	{
		auto [ids, height] = function(sf::Vector2f(x_pos, y_pos));
		y_pos -= height;
		y_pos -= 1.5 * BLOCK_SIZE * level.GetScale();
		pause_menu_ids.insert(pause_menu_ids.end(), ids.begin(), ids.end());
	}

	for (auto id : pause_menu_ids)
	{
		level.AddComponent<PauseMenuItem>(id);
	}
}