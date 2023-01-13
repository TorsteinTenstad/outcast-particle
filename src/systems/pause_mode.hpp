#include "game.hpp"
#include "utils.hpp"

class PauseMode : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{}
	void AddFloatingButtons(Level& level, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts, std::vector<sf::Keyboard::Key> equivalent_keys)
	{
		float button_scale = level.size.x / MENU_LEVEL_WIDTH;
		float button_w = 3072 * button_scale;
		float button_h = 432 * button_scale;
		int text_size = 300 * button_scale;
		auto button_positions = GridHelper(button_texts.size(), 1, button_w, button_h, 200 * button_scale);
		std::string image_path_suffix = "menu_wide.png";
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
			level.GetComponent<CanReceivePress>()[id] = {};
			level.GetComponent<Button>()[id].on_click = button_functions[i];
			level.GetComponent<Button>()[id].image_path = image_path;
			level.GetComponent<Button>()[id].equivalent_key = equivalent_keys[i];
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
		std::vector<std::string> pause_texts = { "[Enter] Continue", "[Backspace] Restart", "[ESC] Return to Menu" };
		std::vector<sf::Keyboard::Key> equivalent_keys = { sf::Keyboard::Enter, sf::Keyboard::Backspace, sf::Keyboard::Escape };
		AddFloatingButtons(level, pause_functions, pause_texts, equivalent_keys);
	}

	void AddLevelFailedButtons(Level& level, std::function<void(int)> set_level, std::function<void(void)> reset_active_level)
	{
		std::vector<std::function<void(void)>> level_failed_functions = { reset_active_level, std::bind(set_level, MAIN_MENU) };
		std::vector<std::string> level_failed_texts = { "[Backspace] Restart", "[ESC] Return to Menu" };
		std::vector<sf::Keyboard::Key> equivalent_keys = { sf::Keyboard::Backspace, sf::Keyboard::Escape };
		AddFloatingButtons(level, level_failed_functions, level_failed_texts, equivalent_keys);
	}

	void AddLevelCompletedButtons(Level& level, int active_level, std::function<void(int)> set_level, std::function<void(Mode)> set_mode, std::function<void(void)> reset_active_level)
	{
		std::vector<std::function<void(void)>> level_completed_functions = { std::bind(set_level, active_level + 1), reset_active_level, std::bind(set_level, MAIN_MENU) };
		std::vector<std::string> level_completed_texts = { "[Tab] Next level", "[Backspace] Restart", "[ESC] Return to Menu" };
		std::vector<sf::Keyboard::Key> equivalent_keys = { sf::Keyboard::Tab, sf::Keyboard::Backspace, sf::Keyboard::Escape };
		AddFloatingButtons(level, level_completed_functions, level_completed_texts, equivalent_keys);
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};