#include "components/draw_info.hpp"
#include "components/level_menu.hpp"
#include "components/physics.hpp"
#include "components/position.hpp"
#include "components/scroll.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "entity_creation.hpp"
#include "game.hpp"
#include "level.hpp"
#include "utils/container_operations.hpp"
#include "utils/string_manip.hpp"

void Game::GoToMainMenu()
{
	Level& level = active_level_.GetActiveState();
	active_level_id_ = MAIN_MENU;
	is_in_level_editing_ = false;
	level.ResetSize();
	sf::Vector2f level_size = level.GetSize();

	float x_center_offset = -8 * BLOCK_SIZE;
	float y_offset = level_size.y - 6.5 * BLOCK_SIZE;

	std::vector<EntitiesHandle> entities_handles;
	auto AddButton = [&](std::function<void(void)> button_function, std::string button_text) {
		EntityHandle button_handle = CreateNavigatorButton(level, sf::Vector2f(0, 0), button_function, button_text, sf::Keyboard::Unknown);
		entities_handles.push_back(ToEntitiesHandle(button_handle));
	};

	AddButton(std::bind(&Game::SetLevel, this, LEVEL_MENU), "Play");
	AddButton(std::bind(&Game::SetLevel, this, OPTIONS_MENU), "Options");
	AddButton([]() {}, "Credits");
	AddButton(std::bind(&Game::ExitGame, this), "Exit Game");

	auto [ids, height] = VerticalEntityLayout(level, sf::Vector2f(level_size.x / 2 + x_center_offset, y_offset), entities_handles, BLOCK_SIZE);

	CreateMenuNavigator(level);

	auto [title_entity_id, title_text, title_draw_priority, title_position] = level.CreateEntityWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Volatile\n  Particle";
	title_position->position.x = level_size.x / 2.f + x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;

	int static_particle_id = level.AddBlueprint(BPStaticParticle);
	level.GetComponent<Position>(static_particle_id)->position = sf::Vector2f(level_size.x / 2.f - x_center_offset, y_offset);

	int player_id = level.AddBlueprint(BPPlayer);
	level.GetComponent<Position>(player_id)->position = sf::Vector2f(level_size.x / 2.f - x_center_offset, y_offset - 3.5 * BLOCK_SIZE);
	level.GetComponent<Velocity>(player_id)->velocity = sf::Vector2f(460, 0);
	level.GetComponent<Charge>(player_id)->charge *= -1;
}

void Game::GoToLevelMenu()
{
	Level& level = active_level_.GetActiveState();
	active_level_id_ = LEVEL_MENU;
	level.ResetSize();
	level.CreateEntityWith<ReDrawLevelMenuRequest>();
}

void Game::GoToOptionsMenu()
{
	Level& level = active_level_.GetActiveState();
	active_level_id_ = OPTIONS_MENU;
	level.ResetSize();
	sf::Vector2f level_size = level.GetSize();

	float x_center_offset = -8 * BLOCK_SIZE;
	float y_offset = level_size.y - 6.5 * BLOCK_SIZE;

	std::vector<EntitiesHandle> entities_handles;
	auto AddButton = [&](std::string level_id, std::string button_text) {
		EntityHandle button_handle = CreateNavigatorButton(level, sf::Vector2f(0, 0), std::bind(&Game::SetLevel, this, level_id), button_text, sf::Keyboard::Unknown);
		entities_handles.push_back(ToEntitiesHandle(button_handle));
	};

	AddButton(KEY_CONFIG_MENU, "Key Config");
	AddButton(MUSIC_AND_SOUND_MENU, "Music & Sound");
	AddButton(GRAPHICS_AND_DISPLAY_MENU, "Display & Graphics");
	AddButton(MAIN_MENU, "Main Menu");

	auto [ids, height] = VerticalEntityLayout(level, sf::Vector2f(level_size.x / 2 + x_center_offset, y_offset), entities_handles, BLOCK_SIZE);

	CreateMenuNavigator(level);

	auto [title_entity_id, title_text, title_draw_priority, title_position] = level.CreateEntityWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Options";
	title_position->position.x = level_size.x / 2.f + x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;

	int entity_position_x = level_size.x - 8 * BLOCK_SIZE;

	int player_id = level.AddBlueprint(BPPlayer);
	level.GetComponent<Position>(player_id)->position = sf::Vector2f(entity_position_x, 1200);
	level.GetComponent<Velocity>(player_id)->velocity = sf::Vector2f(0, 1000);

	int electric_field_1 = level.AddBlueprint(BPElectricField);
	level.GetComponent<Position>(electric_field_1)->position = sf::Vector2f(entity_position_x, 360);
	level.GetComponent<ElectricField>(electric_field_1)->field_vector = sf::Vector2f(0, 0.25);
	level.GetComponent<WidthAndHeight>(electric_field_1)->width_and_height = sf::Vector2f(480, 240);

	int electric_field_2 = level.AddBlueprint(BPElectricField);
	level.GetComponent<Position>(electric_field_2)->position = sf::Vector2f(entity_position_x, 1800);
	level.GetComponent<ElectricField>(electric_field_2)->field_vector = sf::Vector2f(0, -0.25);
	level.GetComponent<WidthAndHeight>(electric_field_2)->width_and_height = sf::Vector2f(480, 240);
}

static void SetupOptionsSubMenu(Level& level, std::string menu_title, std::function<Level&(void)> set_level, std::vector<std::string> description_texts, std::vector<EntitiesCreator> create_buttons)
{
	assert(description_texts.size() == create_buttons.size());

	sf::Vector2f level_size = level.GetSize();
	sf::Vector2f button_position = sf::Vector2f(1.33 * level_size.x / 2.f, 2 * BLOCK_SIZE);

	//Create menu title
	auto [title_entity_id, title_text, title_draw_priority, title_position] = level.CreateEntityWith<Text, DrawPriority, Position>();
	title_text->size = 150;
	title_text->content = menu_title;
	title_position->position.x = level_size.x / 2.f;
	title_position->position.y = 2 * BLOCK_SIZE;

	//Set up scroll window
	auto [scroll_window_entity_id, scroll_window, width_and_height, position] = level.CreateEntityWith<ScrollWindow, WidthAndHeight, Position>();
	scroll_window->entity_height = 2 * BLOCK_SIZE;
	width_and_height->width_and_height = level_size;
	width_and_height->width_and_height.x -= 8 * BLOCK_SIZE;
	width_and_height->width_and_height.y -= 8 * BLOCK_SIZE;
	position->position = level_size / 2.f;

	//Create buttons, texts and add to scroll window
	for (unsigned i = 0; i < description_texts.size(); ++i)
	{
		button_position.y += 3 * BLOCK_SIZE;
		auto [button_ids, button_height] = create_buttons[i](button_position);
		sf::Vector2f description_position = sf::Vector2f(level_size.x - button_position.x, button_position.y);
		auto [description_id, description_height] = CreateScrollingText(level, description_position, description_texts[i]);
		scroll_window->entities.insert(scroll_window->entities.end(), { description_id });
		scroll_window->entities.insert(scroll_window->entities.end(), button_ids.begin(), button_ids.end());
	}

	//Add menu button
	sf::Vector2f menu_button_position = sf::Vector2f(level_size.x / 2.f, level_size.y - 2 * BLOCK_SIZE);
	CreateMenuButton(level, menu_button_position, std::bind(set_level), "Options Menu");
}

void Game::GoToKeyConfigMenu()
{
	Level& level = active_level_.GetActiveState();
	active_level_id_ = KEY_CONFIG_MENU;
	level.ResetSize();
	sf::Vector2f level_size = level.GetSize();

	std::vector<sf::Keyboard::Key*> keys = { &globals.key_config.PLAYER_SWITCH_CHARGE, &globals.key_config.PLAYER_GO_NEUTRAL, &globals.key_config.PLAYER_MOVE_UP, &globals.key_config.PLAYER_MOVE_LEFT, &globals.key_config.PLAYER_MOVE_DOWN, &globals.key_config.PLAYER_MOVE_RIGHT };
	std::vector<EntitiesCreator> EntitiesCreator;
	for (auto key : keys)
	{
		EntitiesCreator.push_back(std::bind(&CreateKeyConfigButton, std::ref(level), std::placeholders::_1, key));
	}
	std::vector<std::string> description_texts = { "Switch charge", "Go neutral", "Move Up", "Move Left", "Move Down", "Move Right" };
	std::function<std::string(std::string)> left_shift_description_texts = std::bind(&LeftShiftString, std::placeholders::_1, 17);

	SetupOptionsSubMenu(level, "Key Config", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), EntitiesCreator);
}

void Game::GoToGraphicsAndDisplayMenu()
{
	Level& level = active_level_.GetActiveState();
	active_level_id_ = GRAPHICS_AND_DISPLAY_MENU;
	level.ResetSize();
	sf::Vector2f level_size = level.GetSize();

	std::vector<std::string> button_texts = {
		BoolToStringAsEnabledOrDisabled(globals.general_config.fullscreen),
		BoolToStringAsEnabledOrDisabled(globals.general_config.forces_are_visualized),
		BoolToStringAsEnabledOrDisabled(globals.general_config.use_ready_mode),
		BoolToStringAsEnabledOrDisabled(globals.general_config.show_timer),
		BoolToStringAsEnabledOrDisabled(globals.general_config.display_precise_badge_time)
	};
	std::vector<std::function<void(void)>> button_functions = {
		std::bind(&Game::ToggleFullscreen, this),
		[](void) { globals.general_config.forces_are_visualized = !globals.general_config.forces_are_visualized; },
		[](void) { globals.general_config.use_ready_mode = !globals.general_config.use_ready_mode; },
		[](void) { globals.general_config.show_timer = !globals.general_config.show_timer; },
		[](void) { globals.general_config.display_precise_badge_time = !globals.general_config.display_precise_badge_time; }
	};
	std::vector<std::string> description_texts = {
		"Fullscreen",
		"Show Forces",
		"Use Ready Mode",
		"Show timer",
		"Detailed Timer"
	};

	std::vector<EntitiesCreator> EntitiesCreator;
	for (unsigned i = 0; i < button_functions.size(); i++)
	{
		EntitiesCreator.push_back(std::bind(&CreateOptionsButton, std::ref(level), std::placeholders::_1, button_functions[i], button_texts[i]));
	}
	std::function<std::string(std::string)> left_shift_description_texts = std::bind(&LeftShiftString, std::placeholders::_1, 17);

	SetupOptionsSubMenu(level, "Graphics and Display", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), EntitiesCreator);
}

void Game::GoToMusicAndSoundMenu()
{
	Level& level = active_level_.GetActiveState();
	active_level_id_ = MUSIC_AND_SOUND_MENU;
	level.ResetSize();
	sf::Vector2f level_size = level.GetSize();

	std::vector<EntitiesCreator> EntitiesCreator = {
		std::bind(&CreateSliderButton, std::ref(level), std::placeholders::_1, &globals.general_config.sound_volume),
		std::bind(
			&CreateOptionsButton, std::ref(level), std::placeholders::_1, [](void) { globals.general_config.play_ambient_sounds = !globals.general_config.play_ambient_sounds; }, BoolToStringAsEnabledOrDisabled(globals.general_config.play_ambient_sounds))
	};

	std::vector<std::string> description_texts = {
		"SFX Volume",
		"Ambient sounds"
	};

	std::function<std::string(std::string)> left_shift_description_texts = std::bind(&LeftShiftString, std::placeholders::_1, 17);

	SetupOptionsSubMenu(level, "Graphics and Display", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), EntitiesCreator);
}
