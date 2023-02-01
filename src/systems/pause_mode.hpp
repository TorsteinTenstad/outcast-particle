#include "game.hpp"
#include "utils.hpp"

#define PAUSE_MENU_DELAY 2 //seconds

class PauseMode : public GameSystem
{
private:
	std::function<void(std::string)> set_level_;

public:
	using GameSystem::GameSystem;
	void GiveFunctions(std::function<void(std::string)> set_level)
	{
		set_level_ = set_level;
	}
	void Update(Level& level, float dt)
	{
		if (IsMenu(active_level_id_))
		{
			return;
		}

		LevelState level_state = level.ComputeState();
		LevelMode level_mode = level.GetMode();
		if (level_mode != PAUSE_MODE
			&& (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape]
				|| level_state == COMPLETED
				|| level_state == FAILED))
		{
			level.SetMode(PAUSE_MODE);
			MenuDelayTimer* menu_delay_timer = GetSingleton<MenuDelayTimer>(level);
			menu_delay_timer->pevious_mode = level_mode;
			menu_delay_timer->duration = cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape] ? -1 : PAUSE_MENU_DELAY;
			menu_delay_timer->buttons_initialized = false;
		}

		if (level_mode == PAUSE_MODE)
		{
			MenuDelayTimer* menu_delay_timer = GetSingleton<MenuDelayTimer>(level);
			if (!menu_delay_timer->buttons_initialized && (menu_delay_timer->duration < 0 || cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape]))
			{
				AddAppropriateButtons(level, menu_delay_timer->pevious_mode);
				menu_delay_timer->buttons_initialized = true;
			}
			else
			{
				menu_delay_timer->duration -= dt;
			}
		}
		else
		{
			RemovePauseButtons(level);
		}
	}
	void AddAppropriateButtons(Level& level, LevelMode previous_mode)
	{
		LevelState level_state = level.ComputeState();
		std::vector<std::function<void(void)>> functions;
		std::vector<sf::Keyboard::Key> shortcut_keys;
		std::vector<std::string> text;

		if (previous_mode == PLAY_MODE)
		{
			if (level_state == PLAYING)
			{
				text.push_back("Continue");
				functions.push_back([&]() { level.SetMode(PLAY_MODE); });
				shortcut_keys.push_back(sf::Keyboard::Escape);
			}

			if (level_state == COMPLETED && !is_in_level_editing_)
			{
				//text.push_back("Next level");
				//functions.push_back(std::bind(set_level_, active_level_id_ + 1));
			}

			text.push_back("Restart level");
			functions.push_back(std::bind(set_level_, active_level_id_));
			shortcut_keys.push_back(sf::Keyboard::R);

			if (is_in_level_editing_)
			{
				text.push_back("Edit level");
				functions.push_back([&]() { level.SetMode(EDIT_MODE); });
				shortcut_keys.push_back(sf::Keyboard::Unknown);
			}
		}

		if (previous_mode == EDIT_MODE)
		{
			text.push_back("Continue editing");
			functions.push_back([&]() { level.SetMode(EDIT_MODE); });
			shortcut_keys.push_back(sf::Keyboard::Escape);

			text.push_back("Play level");
			functions.push_back([&]() { level.SetMode(PLAY_MODE); });
			shortcut_keys.push_back(sf::Keyboard::Unknown);
		}

		text.push_back("Main menu");
		functions.push_back(std::bind(set_level_, MAIN_MENU));
		shortcut_keys.push_back(sf::Keyboard::Unknown);

		AddFloatingButtons(level, functions, text, shortcut_keys);
	}
	void AddFloatingButtons(Level& level, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts, std::vector<sf::Keyboard::Key> shortcut_keys)
	{
		float button_scale = level.size.x / LEVEL_WIDTHS[1];
		std::vector<int> pause_menu_ids = AddButtonList(level, level.size / 2.f, button_functions, button_texts, shortcut_keys, button_scale, button_scale);
		for (auto id : pause_menu_ids)
		{
			level.GetComponent<PauseMenuItems>()[id];
		}
	}
	void RemovePauseButtons(Level& level)
	{
		auto& pause_menu_items_map = level.GetComponent<PauseMenuItems>();

		for (auto it = level.GetComponent<PauseMenuItems>().cbegin(), next_it = it; it != level.GetComponent<PauseMenuItems>().cend(); it = next_it)
		{
			++next_it;
			auto entity_id = it->first;
			level.DeleteEntity(entity_id);
		}
	}
};