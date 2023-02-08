#include "game.hpp"

void Game::ButtunFuncEditLevel()
{
	is_in_level_editing_ = true;
	SetLevel(LEVEL_MENU);
}

void Game::FillActiveLevelWithMainMenu()
{
	active_level_id_ = MAIN_MENU;
	is_in_level_editing_ = false;
	active_level_.ResetSize();
	sf::Vector2f level_size = active_level_.GetSize();

	float x_center_offset = 8 * BLOCK_SIZE;
	float y_offset = level_size.y - 6.5 * BLOCK_SIZE;

	std::vector<std::function<void(void)>> functions = { std::bind(&Game::SetLevel, this, LEVEL_MENU), std::bind(&Game::ButtunFuncEditLevel, this), std::bind(&Game::SetLevel, this, OPTIONS_MENU), std::bind(&Game::ExitGame, this) };
	std::vector<std::string> text = { "Play", "Level Creator", "Options", "Exit Game" };
	AddButtonList(active_level_, sf::Vector2f(level_size.x / 2 - x_center_offset, y_offset), functions, text);

	auto [title_entity_id, title_text, title_draw_priority, title_position] = active_level_.CreateEntitiyWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Outcast\n   Particle";
	title_position->position = level_size / 2.f;
	title_position->position.x = level_size.x / 2.f - x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;

	int static_particle_id = active_level_.AddBlueprint("BPStaticParticle");
	active_level_.GetComponent<Position>()[static_particle_id].position = sf::Vector2f(level_size.x / 2.f + x_center_offset, y_offset);

	int player_id = active_level_.AddBlueprint("BPPlayer");
	active_level_.GetComponent<Position>()[player_id].position = sf::Vector2f(level_size.x / 2.f + x_center_offset, y_offset - 3.5 * BLOCK_SIZE);
	active_level_.GetComponent<Velocity>()[player_id].velocity = sf::Vector2f(460, 0);
	active_level_.GetComponent<Charge>()[player_id].charge *= -1;
}

void Game::FillActiveLevelWithLevelMenu()
{
	active_level_id_ = LEVEL_MENU;
	active_level_.ResetSize();
	active_level_.GetComponent<LevelMenuUI>()[active_level_.CreateEntityId()];
}

void Game::FillActiveLevelWithOptionsMenu()
{
	active_level_id_ = OPTIONS_MENU;
	active_level_.ResetSize();
	active_level_.IncreaseSize();
	float options_button_w = 3072;
	float options_button_h = 432;
	int options_text_size = 200;
	std::vector<sf::Keyboard::Key*> options_keys = { &globals.key_config.PLAYER_MOVE_UP, &globals.key_config.PLAYER_SWITCH_CHARGE, &globals.key_config.PLAYER_MOVE_LEFT, &globals.key_config.PLAYER_GO_NEUTRAL, &globals.key_config.PLAYER_MOVE_DOWN, &globals.key_config.MENU, &globals.key_config.PLAYER_MOVE_RIGHT, &globals.key_config.EDIT_MODE };
	std::vector<std::string> options_text = { "Up", "Switch charge", "Left", "Neutral", "Down", "Pause", "Right", "Toggle edit mode" };
	auto options_button_positions = GridHelper(options_text.size(), 2, options_button_w, options_button_h, 200);
	for (unsigned i = 0; i < options_text.size(); ++i)
	{
		std::string text = options_text[i] + ": " + HumanName(*options_keys[i]);
		sf::Vector2 button_position = options_button_positions[i] + active_level_.GetSize() / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		AddOptionsButton(active_level_, options_keys[i], x, y, options_button_w, options_button_h, text, options_text_size);
	}
	AddMenuButton(active_level_, std::bind(&Game::SetLevel, this, MAIN_MENU), 3840, 3840, "Main Menu");
}