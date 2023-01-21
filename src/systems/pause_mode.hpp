#include "game.hpp"
#include "utils.hpp"

class PauseMode : public GameSystem
{
private:
	std::function<void(int)> set_level_;

public:
	using GameSystem::GameSystem;
	void GiveFunctions(std::function<void(int)> set_level)
	{
		set_level_ = set_level;
	}
	void Update(Level& level, float dt)
	{
		if (active_level_id_ < 0)
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
			AddAppropriateButtons(level, level_mode);
		}

		if (level.GetMode() != PAUSE_MODE)
		{
			RemovePauseButtons(level);
		}
	}
	void AddAppropriateButtons(Level& level, LevelMode previous_mode)
	{
		LevelState level_state = level.ComputeState();
		std::vector<std::function<void(void)>> functions;
		std::vector<std::string> text;

		if (previous_mode == PLAY_MODE)
		{
			if (level_state == PLAYING)
			{
				text.push_back("Continue");
				functions.push_back([&]() { level.SetMode(PLAY_MODE); });
			}

			if (level_state == COMPLETED && !is_in_level_editing_)
			{
				text.push_back("Next level");
				functions.push_back(std::bind(set_level_, active_level_id_ + 1));
			}

			text.push_back("Restart level");
			functions.push_back(std::bind(set_level_, active_level_id_));

			if (is_in_level_editing_)
			{
				text.push_back("Edit level");
				functions.push_back([&]() { level.SetMode(EDIT_MODE); });
			}
		}

		if (previous_mode == EDIT_MODE)
		{
			text.push_back("Continue editing");
			functions.push_back([&]() { level.SetMode(EDIT_MODE); });

			text.push_back("Play level");
			functions.push_back([&]() { level.SetMode(PLAY_MODE); });
		}

		text.push_back("Main menu");
		functions.push_back(std::bind(set_level_, MAIN_MENU));

		AddFloatingButtons(level, functions, text);
	}
	void AddFloatingButtons(Level& level, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts)
	{
		float button_scale = level.size.x / MENU_LEVEL_WIDTH;
		float button_w = 3072 * button_scale;
		float button_h = 432 * button_scale;
		int text_size = 300 * button_scale;
		auto button_positions = GridHelper(button_texts.size(), 1, button_w, button_h, 200 * button_scale);
		for (unsigned i = 0; i < button_texts.size(); ++i)
		{
			sf::Vector2f button_position = button_positions[i] + level.size / 2.f;
			float x = button_position.x;
			float y = button_position.y;
			int id = level.AddBlueprint("BPButton");
			level.GetComponent<Position>()[id] = { sf::Vector2f(x, y) };
			level.GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(button_w, button_h) };
			level.GetComponent<OnReleasedThisFrame>()[id].func = button_functions[i];
			level.GetComponent<Text>()[id].content = button_texts[i];
			level.GetComponent<Text>()[id].size = text_size;
			level.GetComponent<PauseMenuItems>()[id] = {};
		}
		int navigator_id = level.AddBlueprint("BPMenuNavigator");

		level.GetComponent<WidthAndHeight>()[navigator_id].width_and_height *= button_scale;
		level.GetComponent<PauseMenuItems>()[navigator_id];
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