#include "game.hpp"

Game::Game()
{
	levels_[MAIN_MENU];
	levels_[LEVEL_MENU];
	pause_menu_system_ = PauseMenuSystem(std::bind(&Game::SetLevel, this, std::placeholders::_1));
}

void Game::Init()
{
	int level_id = 0;
	for (const auto& entry : std::experimental::filesystem::directory_iterator("levels/"))
	{
		levels_[level_id].LoadFromFile(entry.path().string());
		level_id++;
	}
	int number_of_levels = level_id;

	float button_w = 1.2 * 1280;
	float button_h = 1.2 * 720;
	auto button_positions = GridHelper(number_of_levels, 4, button_w, button_h, 100);
	for (int i = 0; i < number_of_levels; ++i)
	{
		sf::Vector2 button_position = button_positions[i] + levels_[LEVEL_MENU].size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		std::string level_texture_identifier = GenerateLevelTexture(i);
		levels_[LEVEL_MENU].AddButton(std::bind(&Game::SetLevel, this, i), x, y, button_w, button_h, level_texture_identifier, " ", 0);
	}

	float menu_button_w = 1.2 * 2560;
	float menu_button_h = 1.2 * 360;
	int menu_text_size = 300;
	std::vector<std::function<void(void)>> menu_funtions = { std::bind(&Game::SetLevel, this, LEVEL_MENU), std::bind(&Game::SetLevel, this, MAIN_MENU), std::bind(&Game::SetLevel, this, MAIN_MENU), std::bind(&Game::ExitGame, this) };
	std::vector<std::string> menu_text = { "Level Menu", "Multiplayer", "Options", "Exit Game" };
	auto menu_button_positions = GridHelper(menu_text.size(), 1, menu_button_w, menu_button_h, 200);
	for (unsigned i = 0; i < menu_text.size(); ++i)
	{
		sf::Vector2 button_position = menu_button_positions[i] + levels_[MAIN_MENU].size / 2.f;
		float x = button_position.x;
		float y = button_position.y;
		levels_[MAIN_MENU].AddButton(menu_funtions[i], x, y, menu_button_w, menu_button_h, "content\\textures\\gray.png", menu_text[i], menu_text_size);
	}

	active_level_ = MAIN_MENU;
}

void Game::Update(float dt)
{
	event_system_.Update(cursor_and_keys_);
	pause_menu_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	for (int i = 0; i < physics_ticks_per_frame_; ++i)
	{
		UpdatePhysics(dt / physics_ticks_per_frame_);
	}
	sound_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	mouse_interaction_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	button_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	if (!levels_[active_level_].edit_mode)
	{
		player_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	}
	edit_mode_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	set_draw_info_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	trail_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	screen_shake_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	render_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	if (levels_[active_level_].edit_mode)
	{
		display_velocity_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	}
}
void Game::UpdatePhysics(float dt)
{
	if (!levels_[active_level_].edit_mode)
	{
		electric_force_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
		electric_field_force_system.Update(cursor_and_keys_, levels_[active_level_], dt);
		magnetic_field_force_system.Update(cursor_and_keys_, levels_[active_level_], dt);
		force_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
		acceleration_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
		velocity_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
		intersection_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
		goal_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
		kill_on_intersection_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
		collision_system_.Update(cursor_and_keys_, levels_[active_level_], dt);
	}
}

Game::~Game()
{
	if (levels_[active_level_].edit_mode)
	{
		edit_mode_system_.CloseBlueprintMenu(levels_[active_level_]);
		levels_[active_level_].SaveToFile();
	}
}

void Game::SetLevel(int level)
{
	if (active_level_ >= 0)
	{
		if (levels_[active_level_].edit_mode)
		{
			edit_mode_system_.CloseBlueprintMenu(levels_[active_level_]);
			levels_[active_level_].edit_mode = false;
			levels_[active_level_].SaveToFile();
		}
		levels_[active_level_].LoadFromFile();
		GenerateLevelTexture(active_level_);
	}
	active_level_ = level;
}

std::string Game::GenerateLevelTexture(int level_id)
{
	sf::Texture texture;
	texture.create(globals.render_window.getSize().x, globals.render_window.getSize().y);
	int active_level_before_capture = active_level_;
	active_level_ = level_id;
	Update(0);
	active_level_ = active_level_before_capture;
	texture.update(globals.render_window);
	std::string identifier = "level" + std::to_string(level_id);
	render_system_.RegisterTexture(identifier, texture);
	return identifier;
}

void Game::ExitGame()
{
	globals.render_window.close();
}