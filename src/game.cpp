#include "game.hpp"
#include <chrono>
#include <thread>
#include <string>
#include <filesystem>

using namespace std::chrono_literals;

Level& Game::AddLevel()
{
	return AddLevel(next_available_level_id_++);
}

Level& Game::AddLevel(int id)
{
	assert(levels_.find(id) == levels_.end());
	if (id == next_available_level_id_)
	{
		next_available_level_id_++;
	}
	levels_.emplace(id, Level(id));
	return GetLevel(id);
}

Level& Game::GetLevel(int id)
{
	return levels_.find(id)->second;
}

Level& Game::GetActiveLevel()
{
	return levels_.find(active_level_)->second;
}

Game::Game()
{
	RegisterGameSystem<ModeSystem>().SetGameControlFunctions(std::bind(&Game::SetMode, this, std::placeholders::_1), std::bind(&Game::GetMode, this), std::bind(&Game::InLevel, this));
	RegisterGameSystem<PlayerSystem>();
	RegisterGameSystem<GoalSystem>();
	RegisterGameSystem<KillOnIntersectionSystem>();
	RegisterGameSystem<SoundSystem>();
	RegisterGameSystem<MouseInterationSystem>();
	RegisterGameSystem<SetDrawInfoSystem>();
	RegisterGameSystem<TrailSystem>();
	RegisterGameSystem<BackgroundSystem>();
	RegisterGameSystem<LevelCompletionTimeSystem>().SetLevelCompletionTimeRecords(&level_completion_time_records_);
	RegisterGameSystem<LevelButtonSystem>().SetLevelCompletionTimeRecords(&level_completion_time_records_);
	RegisterGameSystem<VelocityDependentDrawLayerSystem>();
	RegisterGameSystem<RenderTrailSystem>();
	RegisterGameSystem<RenderShapesSystem>();
	RegisterGameSystem<RenderTextSystem>();
	RegisterGameSystem<ForceVisualizationSystem>();
	RegisterGameSystem<SegmentedGlowEffectSystem>();
	RegisterGameSystem<DrawSystem>();
	RegisterGameSystem<DisplayVelocitySystem>();
	RegisterGameSystem<EditModeSystem>();
	RegisterGameSystem<ViewSystem>();
	RegisterGameSystem<ButtonSystem>();
	RegisterGameSystem<PauseMode>();

	RegisterPhysicsGameSystem<ElectricForceSystem>();
	RegisterPhysicsGameSystem<ElectricFieldForceSystem>();
	RegisterPhysicsGameSystem<MagneticFieldForceSystem>();
	RegisterPhysicsGameSystem<ForceSystem>();
	RegisterPhysicsGameSystem<AccelerationSystem>();
	RegisterPhysicsGameSystem<VelocitySystem>();
	RegisterPhysicsGameSystem<IntersectionSystem>();
	RegisterPhysicsGameSystem<CollisionSystem>();

	sf::Vector2f menu_size = sf::Vector2f(MENU_LEVEL_WIDTH, MENU_LEVEL_WIDTH / ASPECT_RATIO);
	AddLevel(MAIN_MENU).size = menu_size;
	AddLevel(LEVEL_MENU).size = menu_size;
	AddLevel(OPTIONS_MENU).size = menu_size;
}

void Game::Init()
{
	//Main menu
	float menu_button_w = 3072;
	float menu_button_h = 432;
	int menu_text_size = 300;
	std::vector<std::function<void(void)>> menu_funtions = { std::bind(&Game::SetLevel, this, LEVEL_MENU), std::bind(&Game::SetLevel, this, OPTIONS_MENU), std::bind(&Game::ToggleFullscreen, this), std::bind(&Game::ExitGame, this) };
	std::vector<std::string> menu_text = { "Level Menu", "Options", "Toggle fullscreen", "Exit Game" };
	auto menu_button_positions = GridHelper(menu_text.size(), 1, menu_button_w, menu_button_h, 200);
	for (unsigned i = 0; i < menu_text.size(); ++i)
	{
		sf::Vector2 button_position = menu_button_positions[i] + GetLevel(MAIN_MENU).size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		GetLevel(MAIN_MENU).AddMenuButton(menu_funtions[i], x, y, menu_button_w, menu_button_h, menu_text[i], menu_text_size);
	}

	//Level menu & level generation
	int level_id = 0;
	const std::filesystem::path level_path{"levels/"};
	for (const auto& entry : std::filesystem::directory_iterator{level_path})
	{
		AddLevel().LoadFromFile(entry.path().string());
		level_id++;
	}
	int number_of_levels = level_id;

	int columns = floor(sqrt(number_of_levels)) + 1;
	float margin = 100;
	float button_w = (MENU_LEVEL_WIDTH - (columns + 1) * margin - 4 * margin) / columns;
	float button_h = button_w / ASPECT_RATIO;
	auto button_positions = GridHelper(number_of_levels, columns, button_w, button_h, margin);
	for (int i = 0; i < number_of_levels; ++i)
	{
		sf::Vector2 button_position = button_positions[i] + GetLevel(LEVEL_MENU).size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		std::string level_texture_identifier = GenerateLevelTexture(i);
		int id = GetLevel(LEVEL_MENU).AddLevelButton(i, std::bind(&Game::SetLevel, this, i), x, y, button_w, button_h, level_texture_identifier);
	}

	//Options menu
	float options_button_w = 3072;
	float options_button_h = 432;
	int options_text_size = 200;
	std::vector<sf::Keyboard::Key*> options_keys = { &globals.key_config.PLAYER_MOVE_UP, &globals.key_config.PLAYER_SWITCH_CHARGE, &globals.key_config.PLAYER_MOVE_LEFT, &globals.key_config.PLAYER_GO_NEUTRAL, &globals.key_config.PLAYER_MOVE_DOWN, &globals.key_config.MENU, &globals.key_config.PLAYER_MOVE_RIGHT, &globals.key_config.EDIT_MODE };
	std::vector<std::string> options_text = { "Up", "Switch charge", "Left", "Neutral", "Down", "Pause", "Right", "Toggle edit mode" };
	auto options_button_positions = GridHelper(options_text.size(), 2, menu_button_w, menu_button_h, 200);
	for (unsigned i = 0; i < options_text.size(); ++i)
	{
		std::string text = options_text[i] + ": " + HumanName(*options_keys[i]);
		sf::Vector2 button_position = options_button_positions[i] + GetLevel(OPTIONS_MENU).size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		GetLevel(OPTIONS_MENU).AddOptionsButton(options_keys[i], x, y, options_button_w, options_button_h, text, options_text_size);
	}
	GetLevel(OPTIONS_MENU).AddMenuButton(std::bind(&Game::SetLevel, this, MAIN_MENU), 3840, 3840, options_button_w, options_button_h, "Main Menu", 200);

	//Pause overlay
	float pause_button_w = 3072;
	float pause_button_h = 432;
	int pause_text_size = 300;
	std::vector<std::function<void(void)>> pause_funtions = { std::bind(&Game::SetLevel, this, LEVEL_MENU), std::bind(&Game::SetLevel, this, OPTIONS_MENU), std::bind(&Game::ToggleFullscreen, this) };
	std::vector<std::string> pause_text = { "Continue", "Restart", "Return to Menu" };
	auto pause_button_positions = GridHelper(options_text.size(), 2, pause_button_w, pause_button_h, 200);

	active_level_ = STARTING_LEVEL;
}

void Game::Update(float dt)
{
	sfml_event_handler_.Update(cursor_and_keys_);
	for (const auto& system_id : game_system_ids_)
	{
		game_systems_[system_id]->Update(GetActiveLevel(), dt);
	}
	if (active_mode_ == PLAY_MODE)
	{
		for (int i = 0; i < physics_ticks_per_frame_; ++i)
		{
			for (const auto& system_id : physics_game_system_ids_)
			{
				game_systems_[system_id]->Update(GetActiveLevel(), dt / physics_ticks_per_frame_);
			}
			cursor_and_keys_.ResetFrameEvents();
		}
	}
}

Game::~Game()
{
	SetMode(PLAY_MODE);
}

void Game::SetLevel(int level)
{
	SetMode(PLAY_MODE);
	for (auto& [type_id, game_system] : game_systems_)
	{
		game_system->OnExitLevel(GetActiveLevel());
	}
	active_level_ = level;
	if (active_level_ >= 0)
	{
		GetActiveLevel().LoadFromFile();
		GenerateLevelTexture(active_level_);
	}
	for (auto& [type_id, game_system] : game_systems_)
	{
		game_system->OnEnterLevel(GetActiveLevel());
	}
}

Mode Game::GetMode()
{
	return active_mode_;
}

void Game::SetMode(Mode next_mode)
{
	if (active_mode_ == next_mode)
	{
		return;
	}
	switch (active_mode_)
	{
		case EDIT_MODE: {
			GetGameSystem<EditModeSystem>().CloseBlueprintMenu(GetActiveLevel());
			GetActiveLevel().SaveToFile();
			break;
		}
		case PLAY_MODE:
			break;
		case PAUSE_MODE:
			GetGameSystem<PauseMode>().RemovePauseButtons(GetActiveLevel());
			break;
		case LEVEL_COMPLETED_MODE:
			GetGameSystem<PauseMode>().RemovePauseButtons(GetActiveLevel());
			break;
		case LEVEL_FAILED_MODE:
			GetGameSystem<PauseMode>().RemovePauseButtons(GetActiveLevel());
			break;
		default:
			assert(false);
	}

	for (auto& [type_id, game_system] : game_systems_)
	{
		game_system->OnExitMode(GetActiveLevel());
	}
	active_mode_ = next_mode;
	for (auto& [type_id, game_system] : game_systems_)
	{
		game_system->OnEnterMode(GetActiveLevel());
	}

	switch (next_mode)
	{
		case EDIT_MODE: {
			GetActiveLevel().LoadFromFile();
			break;
		}
		case PLAY_MODE:
			break;
		case PAUSE_MODE:
			GetGameSystem<PauseMode>().AddPauseButtons(GetActiveLevel(), std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::SetMode, this, std::placeholders::_1), std::bind(&Game::ResetActiveLevel, this));
			break;
		case LEVEL_COMPLETED_MODE:
			GetGameSystem<PauseMode>().AddLevelCompletedButtons(GetActiveLevel(), active_level_, std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::SetMode, this, std::placeholders::_1), std::bind(&Game::ResetActiveLevel, this));
			break;
		case LEVEL_FAILED_MODE:
			GetGameSystem<PauseMode>().AddLevelFailedButtons(GetActiveLevel(), std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::ResetActiveLevel, this));
			break;
		default:
			assert(false);
	}
}

std::string Game::GenerateLevelTexture(int level_id)
{
	sf::Texture texture;
	texture.create(globals.render_window.getSize().x, globals.render_window.getSize().y);
	int active_level_before_capture = active_level_;
	active_level_ = level_id;
	globals.render_window.setView(sf::View(GetLevel(level_id).size / 2.f, GetLevel(level_id).size));
	Update(0);
	active_level_ = active_level_before_capture;
	texture.update(globals.render_window);
	std::string identifier = "level" + std::to_string(level_id);
	GetGameSystem<RenderShapesSystem>().RegisterTexture(identifier, texture);
	return identifier;
}

void Game::ToggleFullscreen()
{
	if (fullscreen_)
	{
		globals.render_window.create(sf::VideoMode(1280, 720), "outcast-particle");
	}
	else
	{
		globals.render_window.create(sf::VideoMode::getFullscreenModes()[0], "outcast-particle", sf::Style::Fullscreen);
	}
	fullscreen_ = !fullscreen_;
}

void Game::ExitGame()
{
	globals.render_window.close();
}

void Game::ResetActiveLevel()
{
	SetLevel(active_level_);
}

bool Game::InLevel()
{
	return active_level_ >= 0;
}
