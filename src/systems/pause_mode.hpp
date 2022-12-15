#include "game.hpp"
#include "utils.hpp"

class PauseMode
{
private:
	std::function<void(int)> set_level_;
	std::function<void(void)> reset_active_level_;
	std::function<void(Mode)> set_mode_;

public:
	PauseMode()
	{}
	PauseMode(std::function<void(int)> set_level, std::function<void(Mode)> set_mode, std::function<void(void)> reset_active_level)
	{
		set_level_ = set_level;
		set_mode_ = set_mode;
		reset_active_level_ = reset_active_level;
	}

	void AddPauseButtons(Level& level)
	{
		float button_w = 3072;
		float button_h = 432;
		int text_size = 300;
		std::vector<std::function<void(void)>> pause_functions = { std::bind(set_mode_, PLAY_MODE), reset_active_level_, std::bind(set_level_, MAIN_MENU) };
		std::vector<std::string> pause_text = { "Continue", "Restart", "Return to Menu" };
		auto button_positions = GridHelper(pause_text.size(), 1, button_w, button_h, 200);
		std::string image_path_suffix = ToString(button_w) + "_" + ToString(button_h) + ".png";
		std::string image_path = "content\\textures_generated\\button_" + image_path_suffix;
		std::string pressed_image_path = "content\\textures_generated\\pressed_button_" + image_path_suffix;

		for (unsigned i = 0; i < pause_text.size(); ++i)
		{
			sf::Vector2f button_position = button_positions[i] + level.size / 2.f;
			float x = button_position.x;
			float y = button_position.y;

			int id = level.CreateEntityId();
			level.GetComponent<DrawInfo>()[id].image_path = image_path;
			level.GetComponent<DrawInfo>()[id].scale_to_fit = true;
			level.GetComponent<DrawPriority>()[id].draw_priority = 50;
			level.GetComponent<Position>()[id] = { sf::Vector2f(x, y) };
			level.GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(button_w, button_h) };
			level.GetComponent<ClickedOn>()[id] = {};
			level.GetComponent<Button>()[id].on_click = pause_functions[i];
			level.GetComponent<Button>()[id].image_path = image_path;
			level.GetComponent<Button>()[id].pressed_image_path = pressed_image_path;
			level.GetComponent<Text>()[id].content = pause_text[i];
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
};