#include "Components/collision.hpp"
#include "components/draw_info.hpp"
#include "components/level_menu.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/scroll.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "entity_creation.hpp"
#include "game.hpp"
#include "level.hpp"
#include "utils/container_operations.hpp"
#include "utils/string_manip.hpp"
#include <tuple>

void Game::GoToMainMenu()
{
	active_level_id_ = MAIN_MENU;
	is_in_level_editing_ = false;
	active_level_->music_path = (MUSIC_DIR / "Genesis.wav").string();
	active_level_->ResetSize();
	sf::Vector2f level_size = active_level_->GetSize();

	float x_center_offset = -8 * BLOCK_SIZE;
	float y_offset = level_size.y - 6.5 * BLOCK_SIZE;

	std::vector<EntitiesHandle> entities_handles;
	auto AddButton = [&](std::function<void(void)> button_function, std::string button_text) {
		EntityHandle button_handle = CreateNavigatorButton(*active_level_, sf::Vector2f(0, 0), button_function, button_text, sf::Keyboard::Unknown);
		entities_handles.push_back(ToEntitiesHandle(button_handle));
	};

	AddButton(std::bind(&Game::SetLevel, this, LEVEL_MENU), "Play");
	AddButton(std::bind(&Game::SetLevel, this, OPTIONS_MENU), "Options");
	AddButton(std::bind(&Game::SetLevel, this, CREDITS_MENU), "Credits");
	AddButton(std::bind(&Game::ExitGame, this), "Exit Game");

	auto [entities, height] = VerticalEntityLayout(*active_level_, sf::Vector2f(level_size.x / 2 + x_center_offset, y_offset), entities_handles, BLOCK_SIZE);

	CreateMenuNavigator(*active_level_);

	auto [title_entity, title_text, title_draw_priority, title_position] = active_level_->CreateEntityWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Volatile\n  Particle";
	title_position->position.x = level_size.x / 2.f + x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;

	Entity static_particle_id = active_level_->AddBlueprint(BPStaticParticle);
	active_level_->GetComponent<Position>(static_particle_id)->position = sf::Vector2f(level_size.x / 2.f - x_center_offset, y_offset);

	Entity player_entity = active_level_->AddBlueprint(BPPlayer);
	active_level_->GetComponent<Player>(player_entity)->can_go_neutral = false;
	active_level_->GetComponent<Position>(player_entity)->position = sf::Vector2f(level_size.x / 2.f - x_center_offset, y_offset - 3.5 * BLOCK_SIZE);
	active_level_->GetComponent<Velocity>(player_entity)->velocity = sf::Vector2f(460, 0);
	active_level_->GetComponent<Charge>(player_entity)->charge *= -1;
}

void Game::GoToLevelMenu()
{
	is_in_level_editing_ = false;
	active_level_id_ = LEVEL_MENU;
	active_level_->ResetSize();
	active_level_->CreateEntityWith<ReDrawLevelMenuRequest>();
}

void Game::GoToOptionsMenu()
{
	active_level_id_ = OPTIONS_MENU;
	active_level_->ResetSize();
	sf::Vector2f level_size = active_level_->GetSize();

	float x_center_offset = -8 * BLOCK_SIZE;
	float y_offset = level_size.y - 6.5 * BLOCK_SIZE;

	std::vector<EntitiesHandle> entities_handles;
	auto AddButton = [&](std::string level_id, std::string button_text) {
		EntityHandle button_handle = CreateNavigatorButton(*active_level_, sf::Vector2f(0, 0), std::bind(&Game::SetLevel, this, level_id), button_text, sf::Keyboard::Unknown);
		entities_handles.push_back(ToEntitiesHandle(button_handle));
	};

	AddButton(KEY_CONFIG_MENU, "Key Config");
	AddButton(MUSIC_AND_SOUND_MENU, "Music & Sound");
	AddButton(GRAPHICS_AND_DISPLAY_MENU, "Display & Graphics");
	AddButton(MAIN_MENU, "Main Menu");

	auto [entities, height] = VerticalEntityLayout(*active_level_, sf::Vector2f(level_size.x / 2 + x_center_offset, y_offset), entities_handles, BLOCK_SIZE);

	CreateMenuNavigator(*active_level_);

	auto [title_entity, title_text, title_draw_priority, title_position] = active_level_->CreateEntityWith<Text, DrawPriority, Position>();
	title_text->size = 250;
	title_text->content = "Options";
	title_position->position.x = level_size.x / 2.f + x_center_offset;
	title_position->position.y = 2 * BLOCK_SIZE;

	int entity_position_x = level_size.x - 8 * BLOCK_SIZE;

	Entity player_entity = active_level_->AddBlueprint(BPPlayer);
	active_level_->GetComponent<Player>(player_entity)->can_go_neutral = false;
	active_level_->GetComponent<Position>(player_entity)->position = sf::Vector2f(entity_position_x, 1200);
	active_level_->GetComponent<Velocity>(player_entity)->velocity = sf::Vector2f(0, 1000);

	Entity electric_field_1 = active_level_->AddBlueprint(BPElectricField);
	active_level_->GetComponent<Position>(electric_field_1)->position = sf::Vector2f(entity_position_x, 360);
	active_level_->GetComponent<ElectricField>(electric_field_1)->field_vector = sf::Vector2f(0, 0.25);
	active_level_->GetComponent<WidthAndHeight>(electric_field_1)->width_and_height = sf::Vector2f(480, 240);

	Entity electric_field_2 = active_level_->AddBlueprint(BPElectricField);
	active_level_->GetComponent<Position>(electric_field_2)->position = sf::Vector2f(entity_position_x, 1800);
	active_level_->GetComponent<ElectricField>(electric_field_2)->field_vector = sf::Vector2f(0, -0.25);
	active_level_->GetComponent<WidthAndHeight>(electric_field_2)->width_and_height = sf::Vector2f(480, 240);
}

void Game::GoToCreditsMenu()
{
	active_level_id_ = CREDITS_MENU;
	active_level_->ResetSize();
	sf::Vector2f level_size = active_level_->GetSize();

	float x_center_offset = -7 * BLOCK_SIZE;
	float y_offset = level_size.y - 8 * BLOCK_SIZE;

	CreateText(*active_level_, sf::Vector2f(level_size.x / 2.f + x_center_offset, 2 * BLOCK_SIZE), "Credits", 250);

	//auto scroll_window_handle = CreateScrollWindow(*active_level_, sf::Vector2f(level_size.x / 2 + x_center_offset, y_offset), sf::Vector2f(level_size.x * 2 / 5, level_size.y - 4 * BLOCK_SIZE), 2.5 * BLOCK_SIZE);

#if 0
	std::vector<std::string> lines = {
		"       Lead Developer  Torstein Tenstad     ",
		"  Assistant Developer  Simen Storesund      ",
		"       Many thanks to  Magne Tenstad        ",
		"                       Aasne Hella          ",
		"                       Alexander Wulfsberg  ",
		"                       Henrik Storesund     ",
		"                       And of course to you!"
	};
#else
	std::vector<std::string> lines = {
		"Lead Developer          Torstein Tenstad",
		"Assistant Developer      Simen Storesund",
		"Many thanks to             Magne Tenstad",
		"                             Aasne Hella",
		"                     Alexander Wulfsberg",
		"                        Henrik Storesund",
		"                   And of course to you!"
	};
#endif
	std::vector<EntitiesHandle> entities_handles;
	for (const auto& line : lines)
	{
		EntityHandle text_handle = CreateText(*active_level_, sf::Vector2f(0, 0), line, 90, sf::Vector2f(0, 180));
		entities_handles.push_back(ToEntitiesHandle(text_handle));
	}

	auto [ids, heights] = VerticalEntityLayout(*active_level_, sf::Vector2f(level_size.x / 2 + x_center_offset, 4 * BLOCK_SIZE), entities_handles, 0, StartEdge);

	float entity_position_x = level_size.x - 8 * BLOCK_SIZE;
	float entity_position_y = level_size.y / 2;

	Entity player = active_level_->AddBlueprint(BPPlayer);
	active_level_->GetComponent<Player>(player)->can_go_neutral = false;
	active_level_->GetComponent<Position>(player)->position = sf::Vector2f(entity_position_x + BLOCK_SIZE, entity_position_y);
	active_level_->GetComponent<Velocity>(player)->velocity = sf::Vector2f(rand() % 4000 - 2000, rand() % 4000 - 2000);

	auto AddWall = [&](sf::Vector2f position, sf::Vector2f width_and_height) {
		Entity entity = active_level_->AddBlueprint(BPWall);
		active_level_->GetComponent<Position>(entity)->position = position + sf::Vector2f(BLOCK_SIZE, 0);
		active_level_->GetComponent<WidthAndHeight>(entity)->width_and_height = width_and_height;
		active_level_->GetComponent<Collision>(entity)->bounce_factor = 1.005;
	};

	AddWall(sf::Vector2f(24 * BLOCK_SIZE, 1 * BLOCK_SIZE), sf::Vector2f(12 * BLOCK_SIZE, BLOCK_SIZE));
	AddWall(sf::Vector2f(entity_position_x, level_size.y - BLOCK_SIZE), sf::Vector2f(12 * BLOCK_SIZE, BLOCK_SIZE));
	AddWall(sf::Vector2f(18 * BLOCK_SIZE, entity_position_y), sf::Vector2f(BLOCK_SIZE, 16 * BLOCK_SIZE));
	AddWall(sf::Vector2f(level_size.x - 2 * BLOCK_SIZE, entity_position_y), sf::Vector2f(BLOCK_SIZE, 16 * BLOCK_SIZE));

	AddWall(sf::Vector2f(level_size.x - 5 * BLOCK_SIZE, level_size.y - 8 * BLOCK_SIZE), sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
	AddWall(sf::Vector2f(level_size.x - 10 * BLOCK_SIZE, level_size.y - 12 * BLOCK_SIZE), sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
	AddWall(sf::Vector2f(level_size.x - 6 * BLOCK_SIZE, level_size.y - 15 * BLOCK_SIZE), sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
	AddWall(sf::Vector2f(level_size.x - 11 * BLOCK_SIZE, level_size.y - 6 * BLOCK_SIZE), sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
	AddWall(sf::Vector2f(level_size.x - 8 * BLOCK_SIZE, level_size.y - 4 * BLOCK_SIZE), sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
}

static void SetupOptionsSubMenu(Level& level, std::string menu_title, std::function<Level&(void)> set_level, std::vector<std::string> description_texts, std::vector<EntitiesCreator> create_buttons)
{
	assert(description_texts.size() == create_buttons.size());

	sf::Vector2f level_size = level.GetSize();
	sf::Vector2f button_position = sf::Vector2f(1.33 * level_size.x / 2.f, 2 * BLOCK_SIZE);

	//Create menu title
	auto [title_entity, title_text, title_draw_priority, title_position] = level.CreateEntityWith<Text, DrawPriority, Position>();
	title_text->size = 150;
	title_text->content = menu_title;
	title_position->position.x = level_size.x / 2.f;
	title_position->position.y = 2 * BLOCK_SIZE;

	//Set up scroll window
	auto [scroll_window_entity, scroll_window, width_and_height, position] = level.CreateEntityWith<ScrollWindow, WidthAndHeight, Position>();
	scroll_window->entity_height = 2 * BLOCK_SIZE;
	width_and_height->width_and_height = level_size - sf::Vector2f(8 * BLOCK_SIZE, 8 * BLOCK_SIZE);
	position->position = level_size / 2.f;

	//Create buttons, texts and add to scroll window
	for (unsigned i = 0; i < description_texts.size(); ++i)
	{
		button_position.y = (5 + 1.5 * i) * BLOCK_SIZE;
		auto [button_entities, button_height] = create_buttons[i](button_position);
		sf::Vector2f description_position = sf::Vector2f(level_size.x - button_position.x, button_position.y);
		auto [description_id, description_height] = CreateScrollingText(level, description_position, description_texts[i], 80);
		scroll_window->entities.insert(scroll_window->entities.end(), { description_id });
		scroll_window->entities.insert(scroll_window->entities.end(), button_entities.begin(), button_entities.end());
	}

	//Add menu button
	sf::Vector2f menu_button_position = sf::Vector2f(level_size.x / 2.f, level_size.y - 2 * BLOCK_SIZE);
	CreateMenuButton(level, menu_button_position, std::bind(set_level), "Options Menu");
}

void Game::GoToKeyConfigMenu()
{
	active_level_id_ = KEY_CONFIG_MENU;
	active_level_->ResetSize();
	sf::Vector2f level_size = active_level_->GetSize();

	std::vector<sf::Keyboard::Key*> keys = { &globals.key_config.PLAYER_SWITCH_CHARGE, &globals.key_config.PLAYER_GO_NEUTRAL, &globals.key_config.PLAYER_MOVE_UP, &globals.key_config.PLAYER_MOVE_LEFT, &globals.key_config.PLAYER_MOVE_DOWN, &globals.key_config.PLAYER_MOVE_RIGHT };
	std::vector<EntitiesCreator> EntitiesCreator;
	for (auto key : keys)
	{
		EntitiesCreator.push_back(std::bind(&CreateKeyConfigButton, std::ref(*active_level_), std::placeholders::_1, key));
	}
	std::vector<std::string> description_texts = { "Switch charge", "Go neutral", "Move Up", "Move Left", "Move Down", "Move Right" };
	std::function<std::string(std::string)> left_shift_description_texts = [](std::string s) { return RightPad(s, 17); };

	SetupOptionsSubMenu(*active_level_, "Key Config", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), EntitiesCreator);
	CreateButton(
		*active_level_, sf::Vector2f(level_size.x / 2.f + 8.f * BLOCK_SIZE, level_size.y - 2.f * BLOCK_SIZE), sf::Vector2f(5.f * BLOCK_SIZE, 2.f * BLOCK_SIZE), std::bind(
																																									&CreateConfirmMenu, std::ref(*active_level_), level_size, "Are you sure you want to \n reset all key-bindings?", [&]() { Game::ResetKeyConfig(); }, 1.f),
		"Reset",
		120.f);
}

void Game::GoToGraphicsAndDisplayMenu()
{
	active_level_id_ = GRAPHICS_AND_DISPLAY_MENU;
	active_level_->ResetSize();
	sf::Vector2f level_size = active_level_->GetSize();

	std::vector<std::string> button_texts = {
		BoolToStringAsEnabledOrDisabled(globals.general_config.fullscreen),
		BoolToStringAsEnabledOrDisabled(globals.general_config.forces_are_visualized),
		BoolToStringAsEnabledOrDisabled(globals.general_config.use_ready_mode),
		BoolToStringAsEnabledOrDisabled(globals.general_config.show_timer),
		BoolToStringAsEnabledOrDisabled(globals.general_config.show_show_tooltips),
		BoolToStringAsEnabledOrDisabled(globals.general_config.display_precise_badge_time)
	};
	std::vector<std::function<void(void)>> button_functions = {
		std::bind(&Game::ToggleFullscreen, this),
		[](void) { globals.general_config.forces_are_visualized = !globals.general_config.forces_are_visualized; },
		[](void) { globals.general_config.use_ready_mode = !globals.general_config.use_ready_mode; },
		[](void) { globals.general_config.show_timer = !globals.general_config.show_timer; },
		[](void) { globals.general_config.show_show_tooltips = !globals.general_config.show_show_tooltips; },
		[](void) { globals.general_config.display_precise_badge_time = !globals.general_config.display_precise_badge_time; }
	};
	std::vector<std::string> description_texts = {
		"Fullscreen",
		"Show Forces",
		"Use Ready Mode",
		"Show timer",
		"Show tooltips",
		"Detailed Timer"
	};

	std::vector<EntitiesCreator> EntitiesCreator;
	for (unsigned i = 0; i < button_functions.size(); i++)
	{
		EntitiesCreator.push_back(std::bind(&CreateOptionsButton, std::ref(*active_level_), std::placeholders::_1, button_functions[i], button_texts[i]));
	}
	std::function<std::string(std::string)> left_shift_description_texts = [](std::string s) { return RightPad(s, 17); };

	SetupOptionsSubMenu(*active_level_, "Graphics and Display", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), EntitiesCreator);
}

void Game::GoToMusicAndSoundMenu()
{
	active_level_id_ = MUSIC_AND_SOUND_MENU;
	active_level_->ResetSize();
	sf::Vector2f level_size = active_level_->GetSize();
	std::array<int, 2> range = { 0, 100 };
	std::vector<EntitiesCreator> EntitiesCreator = {
		std::bind(&CreateSliderButton, std::ref(*active_level_), std::placeholders::_1, sf::Vector2f(5 * BLOCK_SIZE, BLOCK_SIZE), &globals.general_config.sound_volume, range),
		std::bind(&CreateSliderButton, std::ref(*active_level_), std::placeholders::_1, sf::Vector2f(5 * BLOCK_SIZE, BLOCK_SIZE), &globals.general_config.music_volume, range),
		std::bind(&CreateSliderButton, std::ref(*active_level_), std::placeholders::_1, sf::Vector2f(5 * BLOCK_SIZE, BLOCK_SIZE), &globals.general_config.ambient_volume, range),
	};

	std::vector<std::string> description_texts = {
		"SFX Volume",
		"Music Volume",
		"Ambient sounds"
	};

	std::function<std::string(std::string)> left_shift_description_texts = [](std::string s) { return RightPad(s, 17); };

	SetupOptionsSubMenu(*active_level_, "Graphics and Display", std::bind(&Game::SetLevel, this, OPTIONS_MENU), ApplyFuncToVector(description_texts, left_shift_description_texts), EntitiesCreator);
}
