#include "game.hpp"

void Game::ButtunFuncEditLevel()
{
	is_in_level_editing_ = true;
	SetLevel(LEVEL_MENU);
}

void Game::GoToMainMenu()
{
	active_level_id_ = MAIN_MENU;
	is_in_level_editing_ = false;
	active_level_.size = MENU_SIZE;
	int menu_text_size = 300;
	std::vector<std::function<void(void)>> menu_funtions = { std::bind(&Game::SetLevel, this, LEVEL_MENU), std::bind(&Game::ButtunFuncEditLevel, this), std::bind(&Game::SetLevel, this, OPTIONS_MENU), std::bind(&Game::ToggleFullscreen, this), std::bind(&Game::ExitGame, this) };
	std::vector<std::string> menu_text = { "Play", "Edit levels", "Options", "Toggle fullscreen", "Exit Game" };
	auto menu_button_positions = GridHelper(menu_text.size(), 1, 0, 432, 200);
	for (unsigned i = 0; i < menu_text.size(); ++i)
	{
		sf::Vector2 button_position = menu_button_positions[i] + active_level_.size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		AddMenuButton(active_level_, menu_funtions[i], x, y, menu_text[i]);
	}
	active_level_.AddBlueprint("BPMenuNavigator");
}

void Game::GoToLevelMenu()
{
	active_level_id_ = LEVEL_MENU;
	active_level_.size = MENU_SIZE;
	active_level_.GetComponent<LevelMenuUI>()[active_level_.CreateEntityId()];
}

void Game::GoToOptionsMenu()
{
	active_level_id_ = OPTIONS_MENU;
	active_level_.size = MENU_SIZE;
	float options_button_w = 3072;
	float options_button_h = 432;
	int options_text_size = 200;
	std::vector<sf::Keyboard::Key*> options_keys = { &globals.key_config.PLAYER_MOVE_UP, &globals.key_config.PLAYER_SWITCH_CHARGE, &globals.key_config.PLAYER_MOVE_LEFT, &globals.key_config.PLAYER_GO_NEUTRAL, &globals.key_config.PLAYER_MOVE_DOWN, &globals.key_config.MENU, &globals.key_config.PLAYER_MOVE_RIGHT, &globals.key_config.EDIT_MODE };
	std::vector<std::string> options_text = { "Up", "Switch charge", "Left", "Neutral", "Down", "Pause", "Right", "Toggle edit mode" };
	auto options_button_positions = GridHelper(options_text.size(), 2, options_button_w, options_button_h, 200);
	for (unsigned i = 0; i < options_text.size(); ++i)
	{
		std::string text = options_text[i] + ": " + HumanName(*options_keys[i]);
		sf::Vector2 button_position = options_button_positions[i] + active_level_.size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		AddOptionsButton(active_level_, options_keys[i], x, y, options_button_w, options_button_h, text, options_text_size);
	}
	AddMenuButton(active_level_, std::bind(&Game::SetLevel, this, MAIN_MENU), 3840, 3840, "Main Menu");
}