#include "game.hpp"

void Game::ButtonFuncEditLevel()
{
	is_in_level_editing_ = true;
	SetLevel(LEVEL_MENU);
}

void Game::GoToMainMenu()
{
	active_level_id_ = MAIN_MENU;
	is_in_level_editing_ = false;
	active_level_.ResetSize();
	sf::Vector2f level_size = active_level_.GetSize();

	float x_center_offset = 8 * BLOCK_SIZE;
	float y_offset = level_size.y - 6.5 * BLOCK_SIZE;

	std::vector<std::function<void(void)>> functions = { std::bind(&Game::SetLevel, this, LEVEL_MENU), std::bind(&Game::ButtonFuncEditLevel, this), std::bind(&Game::SetLevel, this, OPTIONS_MENU), std::bind(&Game::ExitGame, this) };
	std::vector<std::string> text = { "Play", "Level Creator", "Options", "Exit Game" };
	AddButtonList(active_level_, sf::Vector2f(level_size.x / 2 - x_center_offset, y_offset), functions, text);

	auto [title_entity_id, title_text, title_draw_priority, title_position] = active_level_.CreateEntitiyWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Outcast\n   Particle";
	title_position->position.x = level_size.x / 2.f - x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;

	int static_particle_id = active_level_.AddBlueprint(BPStaticParticle);
	active_level_.GetComponent<Position>(static_particle_id)->position = sf::Vector2f(level_size.x / 2.f + x_center_offset, y_offset);

	int player_id = active_level_.AddBlueprint(BPPlayer);
	active_level_.GetComponent<Position>(player_id)->position = sf::Vector2f(level_size.x / 2.f + x_center_offset, y_offset - 3.5 * BLOCK_SIZE);
	active_level_.GetComponent<Velocity>(player_id)->velocity = sf::Vector2f(460, 0);
	active_level_.GetComponent<Charge>(player_id)->charge *= -1;
}

void Game::GoToLevelMenu()
{
	active_level_id_ = LEVEL_MENU;
	active_level_.ResetSize();
	active_level_.CreateEntitiyWith<LevelMenuUI>();
}

void Game::GoToOptionsMenu()
{
	active_level_id_ = OPTIONS_MENU;
	active_level_.ResetSize();
	sf::Vector2f level_size = active_level_.GetSize();

	float x_center_offset = -8 * BLOCK_SIZE;
	float y_offset = level_size.y - 6.5 * BLOCK_SIZE;

	std::vector<std::function<void(void)>> functions = { std::bind(&Game::SetLevel, this, KEY_CONFIG_MENU), std::bind(&Game::SetLevel, this, OPTIONS_MENU), std::bind(&Game::SetLevel, this, GRAPHICS_AND_DISPLAY_MENU), std::bind(&Game::SetLevel, this, MAIN_MENU) };
	std::vector<std::string> text = { "Key Config", "Music & Sound", "Display & Graphics", "Main Menu" };
	AddButtonList(active_level_, sf::Vector2f(level_size.x / 2 + x_center_offset, y_offset), functions, text);

	auto [title_entity_id, title_text, title_draw_priority, title_position] = active_level_.CreateEntitiyWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Options";
	title_position->position.x = level_size.x / 2.f + x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;

	int entity_position_x = level_size.x - 8 * BLOCK_SIZE;

	int player_id = active_level_.AddBlueprint(BPPlayer);
	active_level_.AddComponent<Position>(player_id)->position = sf::Vector2f(entity_position_x, 1200);
	active_level_.AddComponent<Velocity>(player_id)->velocity = sf::Vector2f(0, 1000);

	int electric_field_1 = active_level_.AddBlueprint(BPElectricField);
	active_level_.AddComponent<Position>(electric_field_1)->position = sf::Vector2f(entity_position_x, 360);
	active_level_.AddComponent<ElectricField>(electric_field_1)->field_vector = sf::Vector2f(0, 0.25);
	active_level_.AddComponent<WidthAndHeight>(electric_field_1)->width_and_height = sf::Vector2f(480, 240);

	int electric_field_2 = active_level_.AddBlueprint(BPElectricField);
	active_level_.AddComponent<Position>(electric_field_2)->position = sf::Vector2f(entity_position_x, 1800);
	active_level_.AddComponent<ElectricField>(electric_field_2)->field_vector = sf::Vector2f(0, -0.25);
	active_level_.AddComponent<WidthAndHeight>(electric_field_2)->width_and_height = sf::Vector2f(480, 240);
}

void Game::GoToKeyConfigMenu()
{
	active_level_id_ = KEY_CONFIG_MENU;
	active_level_.ResetSize();
	sf::Vector2f level_size = active_level_.GetSize();

	auto [scroll_window_entity_id, scroll_window, width_and_height, position] = active_level_.CreateEntitiyWith<ScrollWindow, WidthAndHeight, Position>();
	scroll_window->entity_height = 2 * BLOCK_SIZE;
	width_and_height->width_and_height = level_size;
	width_and_height->width_and_height.y -= 8 * BLOCK_SIZE;
	position->position = level_size / 2.f;

	std::vector<sf::Keyboard::Key*> keys = { &globals.key_config.PLAYER_MOVE_UP, &globals.key_config.PLAYER_MOVE_LEFT, &globals.key_config.PLAYER_MOVE_DOWN, &globals.key_config.PLAYER_MOVE_RIGHT, &globals.key_config.PLAYER_SWITCH_CHARGE, &globals.key_config.PLAYER_GO_NEUTRAL, &globals.key_config.MENU };
	std::vector<std::string> button_description = { "Up", "Left", "Down", "Right", "Switch charge", "Neutral", "Pause" };
	assert(keys.size() == button_description.size());

	auto button_positions = GridHelper(button_description.size(), 1, 0, 120);
	for (unsigned i = 0; i < button_description.size(); ++i)
	{
		sf::Vector2 button_position = button_positions[i] + level_size / 2.f;
		button_position.y += 0.3 * level_size.y;
		button_position.x *= 1.33;
		int button_id = AddOptionsButton(active_level_, keys[i], button_position.x, button_position.y, "");
		int button_text_id = AddScrollingText(active_level_, button_position.x, button_position.y, HumanName(*keys[i]));
		active_level_.AddComponent<KeyConfigButton>(button_id)->button_text = &active_level_.AddComponent<Text>(button_text_id)->content;
		float description_position = level_size.x - button_position.x;
		int description_id = AddScrollingText(active_level_, description_position, button_position.y, OptionsDescriptionTextSetter(button_description[i]));
		scroll_window->entities.insert(scroll_window->entities.end(), { button_id, button_text_id, description_id });
	}
	int menu_button_position_x = level_size.x / 2.f;
	int menu_button_position_y = level_size.y - 2 * BLOCK_SIZE;
	AddMenuButton(active_level_, std::bind(&Game::SetLevel, this, OPTIONS_MENU), menu_button_position_x, menu_button_position_y, "Options Menu");

	auto [title_entity_id, title_text, title_draw_priority, title_position] = active_level_.CreateEntitiyWith<Text, DrawPriority, Position>();
	title_text->size = 150;
	title_text->content = "Key Config";
	title_position->position.x = level_size.x / 2.f;
	title_position->position.y = 2 * BLOCK_SIZE;
}

void Game::GoToGraphicsAndDisplayMenu()
{
	active_level_id_ = GRAPHICS_AND_DISPLAY_MENU;
	active_level_.ResetSize();
	sf::Vector2f level_size = active_level_.GetSize();

	/*auto [scroll_window_entity_id, scroll_window, width_and_height, position] = active_level_.CreateEntitiyWith<ScrollWindow, WidthAndHeight, Position>();
	scroll_window->entity_height = 2 * BLOCK_SIZE;
	width_and_height->width_and_height = level_size;
	width_and_height->width_and_height.y -= 8 * BLOCK_SIZE;
	position->position = level_size / 2.f;*/

	float x_center_offset = 0 * BLOCK_SIZE;
	float y_offset = level_size.y - 6.5 * BLOCK_SIZE;

	std::vector<std::function<void(void)>> functions = { std::bind(&Game::SetLevel, this, KEY_CONFIG_MENU), std::bind(&Game::SetLevel, this, OPTIONS_MENU), std::bind(&Game::SetLevel, this, GRAPHICS_AND_DISPLAY_MENU), std::bind(&Game::SetLevel, this, MAIN_MENU) };
	std::vector<std::string> text = { "Key Config", "Music & Sound", "Display & Graphics", "Main Menu" };
	AddButtonList(active_level_, sf::Vector2f(level_size.x / 2 + x_center_offset, y_offset), functions, text);

	auto [title_entity_id, title_text, title_draw_priority, title_position] = active_level_.CreateEntitiyWith<Text, DrawPriority, Position>();
	title_text->size = 150;
	title_text->content = "Graphics and Display";
	title_position->position.x = level_size.x / 2.f + x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;
}