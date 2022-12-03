#include "game.hpp"

Game::Game()
{
	cursor_and_keys_ = CursorAndKeys();
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
	sf::Texture texture;
	std::string identifier;

	float button_w = 1.2 * 1280;
	float button_h = 1.2 * 720;
	float spacing = 100;
	float n_columns = 4;
	int c = 0;
	int r = 0;
	for (int i = 0; i < level_id; ++i)
	{
		texture.create(globals.render_window.getSize().x, globals.render_window.getSize().y);
		globals.render_window.clear();
		active_level = i;
		Update(0);
		texture.update(globals.render_window);
		identifier = "level" + std::to_string(i);
		render_system_.RegisterTexture(identifier, texture);
		levels_[LEVEL_MENU].AddButton(std::bind(&Game::SetLevel, this, i), spacing + button_w / 2 + (button_w + spacing) * c, spacing + button_h / 2 + (button_h + spacing) * r, button_w, button_h, identifier, "");

		c++;
		if (c == n_columns)
		{
			r++;
			c = 0;
		}
	}
	float menu_button_w = 1.2 * 2560;
	float menu_button_h = 1.2 * 360;
	float menu_spacing = 200;
	float menu_n_columns = 1;
	int menu_r = 0;
	std::vector<std::function<void(void)>> menu_reference = { std::bind(&Game::SetLevel, this, LEVEL_MENU), std::bind(&Game::SetLevel, this, MAIN_MENU), std::bind(&Game::SetLevel, this, MAIN_MENU), std::bind(&Game::ExitGame, this) };

	for (unsigned i = 0; i < 4; ++i)
	{
		levels_[MAIN_MENU].AddButton(menu_reference[i], 7680 / 2, 700 + menu_spacing + menu_button_h / 2 + (menu_button_h + menu_spacing) * menu_r, menu_button_w, menu_button_h, "content\\textures\\gray.png", "humlesnurr");
		menu_r++;
	}
	active_level = MAIN_MENU;
}

void Game::Update(float dt)
{
	event_system_.Update(cursor_and_keys_);
	pause_menu_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	for (int i = 0; i < physics_ticks_per_frame_; ++i)
	{
		UpdatePhysics(dt / physics_ticks_per_frame_);
	}
	sound_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	mouse_interaction_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	button_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	if (!levels_[active_level].edit_mode)
	{
		player_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	}
	edit_mode_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	set_draw_info_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	trail_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	screen_shake_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	render_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	if (levels_[active_level].edit_mode)
	{
		display_velocity_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	}
}
void Game::UpdatePhysics(float dt)
{
	if (!levels_[active_level].edit_mode)
	{
		electric_force_system_.Update(cursor_and_keys_, levels_[active_level], dt);
		electric_field_force_system.Update(cursor_and_keys_, levels_[active_level], dt);
		magnetic_field_force_system.Update(cursor_and_keys_, levels_[active_level], dt);
		force_system_.Update(cursor_and_keys_, levels_[active_level], dt);
		acceleration_system_.Update(cursor_and_keys_, levels_[active_level], dt);
		velocity_system_.Update(cursor_and_keys_, levels_[active_level], dt);
		intersection_system_.Update(cursor_and_keys_, levels_[active_level], dt);
		goal_system_.Update(cursor_and_keys_, levels_[active_level], dt);
		kill_on_intersection_system_.Update(cursor_and_keys_, levels_[active_level], dt);
		collision_system_.Update(cursor_and_keys_, levels_[active_level], dt);
	}
}

Game::~Game()
{
	if (levels_[active_level].edit_mode)
	{
		edit_mode_system_.CloseBlueprintMenu(levels_[active_level]);
		levels_[active_level].SaveToFile();
	}
}

void Game::SetLevel(int level)
{
	if (active_level >= 0)
	{
		if (levels_[active_level].edit_mode)
		{
			edit_mode_system_.CloseBlueprintMenu(levels_[active_level]);
			levels_[active_level].SaveToFile();
			levels_[active_level].edit_mode = false;
		}
		levels_[active_level].LoadFromFile();
	}
	active_level = level;
}

void Game::ExitGame()
{
	globals.render_window.close();
}