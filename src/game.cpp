#include "game.hpp"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

Level* Game::AddLevel()
{
	return AddLevel(next_available_level_id_++);
}

Level* Game::AddLevel(int id)
{
	assert(levels_.find(id) == levels_.end());
	if (id == next_available_level_id_)
	{
		next_available_level_id_++;
	}
	levels_.emplace(id, Level(id));
	return &levels_.find(id)->second;
}

Game::Game()
{
	sf::Vector2f menu_size = sf::Vector2f(MENU_LEVEL_WIDTH, MENU_LEVEL_WIDTH / ASPECT_RATIO);
	AddLevel(MAIN_MENU)->size = menu_size;
	AddLevel(LEVEL_MENU)->size = menu_size;
	AddLevel(OPTIONS_MENU)->size = menu_size;

	level_button_system_ = LevelButtonSystem(&level_completion_time_records_);
	level_completion_time_system_ = LevelCompletionTimeSystem(&level_completion_time_records_);

	mode_system_ = ModeSystem(std::bind(&Game::SetMode, this, std::placeholders::_1), std::bind(&Game::GetMode, this), std::bind(&Game::InLevel, this));
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
		sf::Vector2 button_position = menu_button_positions[i] + levels_.find(MAIN_MENU)->second.size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		levels_.find(MAIN_MENU)->second.AddMenuButton(menu_funtions[i], x, y, menu_button_w, menu_button_h, menu_text[i], menu_text_size);
	}

	//Level menu & level generation
	int level_id = 0;
	for (const auto& entry : std::experimental::filesystem::directory_iterator("levels/"))
	{
		AddLevel()->LoadFromFile(entry.path().string());
		level_id++;
	}
	int number_of_levels = level_id;

	float button_w = 1.2 * 1280;
	float button_h = 1.2 * 720;
	auto button_positions = GridHelper(number_of_levels, 4, button_w, button_h, 100);
	for (int i = 0; i < number_of_levels; ++i)
	{
		sf::Vector2 button_position = button_positions[i] + levels_.find(LEVEL_MENU)->second.size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		std::string level_texture_identifier = GenerateLevelTexture(i);
		int id = levels_.find(LEVEL_MENU)->second.AddLevelButton(i, std::bind(&Game::SetLevel, this, i), x, y, button_w, button_h, level_texture_identifier);
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
		sf::Vector2 button_position = options_button_positions[i] + levels_.find(OPTIONS_MENU)->second.size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		levels_.find(OPTIONS_MENU)->second.AddOptionsButton(options_keys[i], x, y, options_button_w, options_button_h, text, options_text_size);
	}
	levels_.find(OPTIONS_MENU)->second.AddMenuButton(std::bind(&Game::SetLevel, this, MAIN_MENU), 3840, 3840, options_button_w, options_button_h, "Main Menu", 200);

	//Pause overlay
	float pause_button_w = 3072;
	float pause_button_h = 432;
	int pause_text_size = 300;
	std::vector<std::function<void(void)>> pause_funtions = { std::bind(&Game::SetLevel, this, LEVEL_MENU), std::bind(&Game::SetLevel, this, OPTIONS_MENU), std::bind(&Game::ToggleFullscreen, this) };
	std::vector<std::string> pause_text = { "Continue", "Restart", "Return to Menu" };
	auto pause_button_positions = GridHelper(options_text.size(), 2, pause_button_w, pause_button_h, 200);

	active_level_ = MAIN_MENU;
}

void Game::Update(float dt)
{
	event_system_.Update(cursor_and_keys_);
	mode_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	if (active_mode_ == PLAY_MODE)
	{
		player_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
		for (int i = 0; i < physics_ticks_per_frame_; ++i)
		{
			UpdatePhysics(dt / physics_ticks_per_frame_);
		}
	}
	sound_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	mouse_interaction_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	button_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	set_draw_info_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	trail_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	background_system_.Update(active_mode_, cursor_and_keys_, levels_.find(active_level_)->second, dt);
	level_completion_time_system_.Update(active_mode_, cursor_and_keys_, levels_.find(active_level_)->second, dt);
	level_button_system_.Update(active_mode_, cursor_and_keys_, levels_.find(active_level_)->second, dt);
	render_trail_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	render_shapes_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	render_text_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	draw_system_.Update(active_mode_, cursor_and_keys_, levels_.find(active_level_)->second, dt);
	if (active_mode_ == EDIT_MODE)
	{
		display_velocity_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
		edit_mode_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	}
	screen_shake_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
}
void Game::UpdatePhysics(float dt)
{
	electric_force_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	electric_field_force_system.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	magnetic_field_force_system.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	force_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	acceleration_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	velocity_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	intersection_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	goal_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	kill_on_intersection_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
	collision_system_.Update(cursor_and_keys_, levels_.find(active_level_)->second, dt);
}

Game::~Game()
{
	SetMode(PLAY_MODE);
}

void Game::SetLevel(int level)
{
	SetMode(PLAY_MODE);
	if (active_level_ >= 0)
	{
		levels_.find(active_level_)->second.LoadFromFile();
		GenerateLevelTexture(active_level_);
	}
	active_level_ = level;
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
			edit_mode_system_.CloseBlueprintMenu(levels_.find(active_level_)->second);
			levels_.find(active_level_)->second.SaveToFile();
			break;
		}
		case PLAY_MODE:
			break;
		case PAUSE_MODE:
			pause_mode_.RemovePauseButtons(levels_.find(active_level_)->second);
			break;
		case LEVEL_COMPLETED_MODE:
			pause_mode_.RemovePauseButtons(levels_.find(active_level_)->second);
			break;
		case LEVEL_FAILED_MODE:
			pause_mode_.RemovePauseButtons(levels_.find(active_level_)->second);
			break;
		default:
			assert(false);
	}
	active_mode_ = next_mode;
	switch (next_mode)
	{
		case EDIT_MODE: {
			levels_.find(active_level_)->second.LoadFromFile();
			break;
		}
		case PLAY_MODE:
			break;
		case PAUSE_MODE:
			pause_mode_.AddPauseButtons(levels_.find(active_level_)->second, std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::SetMode, this, std::placeholders::_1), std::bind(&Game::ResetActiveLevel, this));
			break;
		case LEVEL_COMPLETED_MODE:
			pause_mode_.AddLevelCompletedButtons(levels_.find(active_level_)->second, active_level_, std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::SetMode, this, std::placeholders::_1), std::bind(&Game::ResetActiveLevel, this));
			break;
		case LEVEL_FAILED_MODE:
			pause_mode_.AddLevelFailedButtons(levels_.find(active_level_)->second, std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::ResetActiveLevel, this));
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
	globals.render_window.setView(sf::View(levels_.find(level_id)->second.size / 2.f, levels_.find(level_id)->second.size));
	Update(0);
	active_level_ = active_level_before_capture;
	texture.update(globals.render_window);
	std::string identifier = "level" + std::to_string(level_id);
	render_shapes_system_.RegisterTexture(identifier, texture);
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
