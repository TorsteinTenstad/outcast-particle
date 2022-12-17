#include "game.hpp"
#include "utils.hpp"

class PauseMode
{

public:
	void AddFloatingButtons(Level& level, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts)
	{
		float button_w = 3072;
		float button_h = 432;
		int text_size = 300;
		auto button_positions = GridHelper(button_texts.size(), 1, button_w, button_h, 200);
		std::string image_path_suffix = ToString(button_w) + "_" + ToString(button_h) + ".png";
		std::string image_path = "content\\textures_generated\\button_" + image_path_suffix;
		std::string pressed_image_path = "content\\textures_generated\\pressed_button_" + image_path_suffix;
		for (unsigned i = 0; i < button_texts.size(); ++i)
		{
			sf::Vector2f button_position = button_positions[i] + level.size / 2.f;
			float x = button_position.x;
			float y = button_position.y;
			int id = level.CreateEntityId();
			level.GetComponent<DrawInfo>()[id].image_path = image_path;
			level.GetComponent<DrawInfo>()[id].scale_to_fit = true;
			level.GetComponent<DrawPriority>()[id].draw_priority = UI_BASE_DRAW_PRIORITY;
			level.GetComponent<Position>()[id] = { sf::Vector2f(x, y) };
			level.GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(button_w, button_h) };
			level.GetComponent<ClickedOn>()[id] = {};
			level.GetComponent<Button>()[id].on_click = button_functions[i];
			level.GetComponent<Button>()[id].image_path = image_path;
			level.GetComponent<Button>()[id].pressed_image_path = pressed_image_path;
			level.GetComponent<Text>()[id].content = button_texts[i];
			level.GetComponent<Text>()[id].size = text_size;
			level.GetComponent<PauseMenuItems>()[id] = {};
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
	void AddPauseButtons(Level& level, std::function<void(int)> set_level, std::function<void(Mode)> set_mode, std::function<void(void)> reset_active_level)
	{
		std::vector<std::function<void(void)>> pause_functions = { std::bind(set_mode, PLAY_MODE), reset_active_level, std::bind(set_level, MAIN_MENU) };
		std::vector<std::string> pause_texts = { "Continue", "Restart", "Return to Menu" };
		AddFloatingButtons(level, pause_functions, pause_texts);
	}

	void AddLevelFailedButtons(Level& level, std::function<void(int)> set_level, std::function<void(void)> reset_active_level)
	{
		std::vector<std::function<void(void)>> level_failed_functions = { reset_active_level, std::bind(set_level, MAIN_MENU) };
		std::vector<std::string> level_failed_texts = { "Restart", "Return to Menu" };
		AddFloatingButtons(level, level_failed_functions, level_failed_texts);
	}

	void AddLevelCompletedButtons(Level& level, int active_level, std::function<void(int)> set_level, std::function<void(Mode)> set_mode, std::function<void(void)> reset_active_level)
	{
		std::vector<std::function<void(void)>> level_completed_functions = { std::bind(set_level, active_level + 1), reset_active_level, std::bind(set_level, MAIN_MENU) };
		std::vector<std::string> level_completed_texts = { "Next level", "Restart", "Return to Menu" };
		AddFloatingButtons(level, level_completed_functions, level_completed_texts);
	}
};