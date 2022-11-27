#include "game.hpp"

Game::Game()
{
	cursor_and_keys_ = CursorAndKeys();

	Level level_menu = Level();
	Level main_menu = Level();
	levels_.push_back(main_menu);
	levels_.push_back(level_menu);
}

void Game::Init()
{
	for (const auto& entry : std::experimental::filesystem::directory_iterator("levels/"))
	{
		Level level = Level();
		level.LoadFromFile(entry.path().string());
		levels_.push_back(level);
	}
	sf::Texture texture;
	std::string identifier;

	float button_w = 1.2 * 1280;
	float button_h = 1.2 * 720;
	float spacing = 100;
	float n_columns = 4;
	int c = 0;
	int r = 0;
	for (unsigned i = 1; i < levels_.size(); ++i)
	{
		texture.create(globals.render_window.getSize().x, globals.render_window.getSize().y);
		globals.render_window.clear();
		globals.active_level = i;
		Update(0);
		texture.update(globals.render_window);
		identifier = "level" + std::to_string(i);
		render_system_.RegisterTexture(identifier, texture);
		levels_[1].AddLevelButton(i, spacing + button_w / 2 + (button_w + spacing) * c, spacing + button_h / 2 + (button_h + spacing) * r, button_w, button_h, identifier);

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
	std::vector<int> menu_reference = { 1, 0, 1, 0 };
	for (unsigned i = 0; i < 4; ++i)
	{
		identifier = "level" + std::to_string(menu_reference[i]);
		render_system_.RegisterTexture(identifier, texture);
		levels_[0].AddLevelButton(menu_reference[i], 7680 / 2, 700 + menu_spacing + menu_button_h / 2 + (menu_button_h + menu_spacing) * menu_r, menu_button_w, menu_button_h, "content\\textures\\gray.png");
		menu_r++;
	}
	globals.active_level = 0;
}

void Game::Update(float dt)
{
	for (int i = 0; i < physics_ticks_per_frame_; ++i)
	{
		UpdatePhysics(dt / physics_ticks_per_frame_);
	}
	trail_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	screen_shake_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	render_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	if (levels_[globals.active_level].edit_mode)
	{
		display_velocity_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	}
}
void Game::UpdatePhysics(float dt)
{
	event_system_.Update(cursor_and_keys_);
	if (cursor_and_keys_.key_pressed_this_frame[MENU_KEY] && globals.active_level != 0)
	{
		if (levels_[globals.active_level].edit_mode)
		{
			edit_mode_system_.CloseBlueprintMenu(levels_[globals.active_level]);
			levels_[globals.active_level].SaveToFile();
		}
		for (unsigned i = 2; i < levels_.size(); ++i)
		{
			levels_[i].LoadFromFile();
		}
		levels_[globals.active_level].edit_mode = false;
		globals.active_level = 0;
	}
	mouse_interaction_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	level_button_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	if (!levels_[globals.active_level].edit_mode)
	{
		player_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		electric_force_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		electric_field_force_system.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		magnetic_field_force_system.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		force_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		acceleration_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		velocity_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		intersection_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		goal_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		kill_on_intersection_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		collision_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		sound_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	}
	edit_mode_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	set_draw_info_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
}

Game::~Game()
{
	if (levels_[globals.active_level].edit_mode)
	{
		edit_mode_system_.CloseBlueprintMenu(levels_[globals.active_level]);
		levels_[globals.active_level].SaveToFile();
	}
}