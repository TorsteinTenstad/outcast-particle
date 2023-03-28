#include "entity_creation.hpp"
#include "game.hpp"
#include "level.hpp"
#include "utils/container_operations.hpp"
#include "utils/string_manip.hpp"

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
	CreateButtonList(active_level_, sf::Vector2f(level_size.x / 2 - x_center_offset, y_offset), functions, text);

	auto [title_entity_id, title_text, title_draw_priority, title_position] = active_level_.CreateEntitiyWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Volatile\n  Particle";
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

	std::vector<std::function<void(void)>> functions = { std::bind(&Game::SetLevel, this, KEY_CONFIG_MENU), std::bind(&Game::SetLevel, this, MUSIC_AND_SOUND_MENU), std::bind(&Game::SetLevel, this, GRAPHICS_AND_DISPLAY_MENU), std::bind(&Game::SetLevel, this, MAIN_MENU) };
	std::vector<std::string> text = { "Key Config", "Music & Sound", "Display & Graphics", "Main Menu" };
	CreateButtonList(active_level_, sf::Vector2f(level_size.x / 2 + x_center_offset, y_offset), functions, text);

	auto [title_entity_id, title_text, title_draw_priority, title_position] = active_level_.CreateEntitiyWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Options";
	title_position->position.x = level_size.x / 2.f + x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;

	int entity_position_x = level_size.x - 8 * BLOCK_SIZE;

	int player_id = active_level_.AddBlueprint(BPPlayer);
	active_level_.GetComponent<Position>(player_id)->position = sf::Vector2f(entity_position_x, 1200);
	active_level_.GetComponent<Velocity>(player_id)->velocity = sf::Vector2f(0, 1000);

	int electric_field_1 = active_level_.AddBlueprint(BPElectricField);
	active_level_.GetComponent<Position>(electric_field_1)->position = sf::Vector2f(entity_position_x, 360);
	active_level_.GetComponent<ElectricField>(electric_field_1)->field_vector = sf::Vector2f(0, 0.25);
	active_level_.GetComponent<WidthAndHeight>(electric_field_1)->width_and_height = sf::Vector2f(480, 240);

	int electric_field_2 = active_level_.AddBlueprint(BPElectricField);
	active_level_.GetComponent<Position>(electric_field_2)->position = sf::Vector2f(entity_position_x, 1800);
	active_level_.GetComponent<ElectricField>(electric_field_2)->field_vector = sf::Vector2f(0, -0.25);
	active_level_.GetComponent<WidthAndHeight>(electric_field_2)->width_and_height = sf::Vector2f(480, 240);
}

static void SetupOptionsSubMenu(Level& level, std::string menu_title, std::function<Level&(void)> set_level, std::vector<std::string> description_texts, std::vector<entities_creator> create_buttons)
{
	assert(description_texts.size() == create_buttons.size());

	sf::Vector2f level_size = level.GetSize();
	sf::Vector2f button_position = sf::Vector2f(1.33 * level_size.x / 2.f, 2 * BLOCK_SIZE);

	//Create menu title
	auto [title_entity_id, title_text, title_draw_priority, title_position] = level.CreateEntitiyWith<Text, DrawPriority, Position>();
	title_text->size = 150;
	title_text->content = menu_title;
	title_position->position.x = level_size.x / 2.f;
	title_position->position.y = 2 * BLOCK_SIZE;

	//Set up scroll window
	auto [scroll_window_entity_id, scroll_window, width_and_height, position] = level.CreateEntitiyWith<ScrollWindow, WidthAndHeight, Position>();
	scroll_window->entity_height = 2 * BLOCK_SIZE;
	width_and_height->width_and_height = level_size;
	width_and_height->width_and_height.y -= 8 * BLOCK_SIZE;
	position->position = level_size / 2.f;

	//Create buttons, texts and add to scroll window
	for (unsigned i = 0; i < description_texts.size(); ++i)
	{
		button_position.y += 3 * BLOCK_SIZE;
		auto [button_ids, button_height] = create_buttons[i](button_position);
		sf::Vector2f description_position = sf::Vector2f(level_size.x - button_position.x, button_position.y);
		auto [description_id, description_height] = CreateScrollingText(level, description_position, description_texts[i]);
		scroll_window->entities.insert(scroll_window->entities.end(), { description_id[0] });
		scroll_window->entities.insert(scroll_window->entities.end(), button_ids.begin(), button_ids.end());
	}

	//Add menu button
	sf::Vector2f menu_button_position = sf::Vector2f(level_size.x / 2.f, level_size.y - 2 * BLOCK_SIZE);
	CreateMenuButton(level, menu_button_position, std::bind(set_level), "Options Menu");
}

void Game::GoToKeyConfigMenu()
{
	active_level_id_ = KEY_CONFIG_MENU;
	active_level_.ResetSize();
	sf::Vector2f level_size = active_level_.GetSize();

	std::vector<sf::Keyboard::Key*> keys = { &globals.key_config.PLAYER_SWITCH_CHARGE, &globals.key_config.PLAYER_GO_NEUTRAL, &globals.key_config.PLAYER_MOVE_UP, &globals.key_config.PLAYER_MOVE_LEFT, &globals.key_config.PLAYER_MOVE_DOWN, &globals.key_config.PLAYER_MOVE_RIGHT };
	std::vector<entities_creator> entities_creator;
	for (auto key : keys)
	{
		entities_creator.push_back(std::bind(&CreateKeyConfigButton, std::ref(active_level_), std::placeholders::_1, key));
	}
	std::vector<std::string> description_texts = { "Switch charge", "Go neutral", "Move Up", "Move Left", "Move Down", "Move Right" };
	std::function<std::string(std::string)> left_shift_description_texts = std::bind(&LeftShiftString, std::placeholders::_1, 17);

	SetupOptionsSubMenu(active_level_, "Key Config", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), entities_creator);
}

void Game::GoToGraphicsAndDisplayMenu()
{
	active_level_id_ = GRAPHICS_AND_DISPLAY_MENU;
	active_level_.ResetSize();
	sf::Vector2f level_size = active_level_.GetSize();

	std::vector<std::string> button_texts = {
		BoolToStringAsEnabledOrDisabled(globals.general_config.fullscreen),
		BoolToStringAsEnabledOrDisabled(globals.general_config.forces_are_visualized),
		BoolToStringAsEnabledOrDisabled(globals.general_config.use_ready_mode)
	};
	std::vector<std::function<void(void)>> button_functions = {
		std::bind(&Game::ToggleFullscreen, this),
		[](void) { globals.general_config.forces_are_visualized = !globals.general_config.forces_are_visualized; },
		[](void) { globals.general_config.use_ready_mode = !globals.general_config.use_ready_mode; }
	};
	std::vector<std::string> description_texts = {
		"Fullscreen",
		"Show Forces",
		"Use Ready Mode"
	};

	std::vector<entities_creator> entities_creator;
	for (unsigned i = 0; i < button_functions.size(); i++)
	{
		entities_creator.push_back(std::bind(&CreateOptionsButton, std::ref(active_level_), std::placeholders::_1, button_functions[i], button_texts[i]));
	}
	std::function<std::string(std::string)> left_shift_description_texts = std::bind(&LeftShiftString, std::placeholders::_1, 17);

	SetupOptionsSubMenu(active_level_, "Graphics and Display", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), entities_creator);
}

void Game::GoToMusicAndSoundMenu()
{
	active_level_id_ = MUSIC_AND_SOUND_MENU;
	active_level_.ResetSize();
	sf::Vector2f level_size = active_level_.GetSize();

	std::vector<entities_creator> entities_creator;

	std::vector<std::string> description_texts = { "SFX Volume" };
	for (auto description_text : description_texts)
	{
		entities_creator.push_back(std::bind(&CreateSliderButton, std::ref(active_level_), std::placeholders::_1, &globals.general_config.sound_volume));
	}

	std::function<std::string(std::string)> left_shift_description_texts = std::bind(&LeftShiftString, std::placeholders::_1, 17);

	SetupOptionsSubMenu(active_level_, "Graphics and Display", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), entities_creator);
}
