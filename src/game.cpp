#include "game.hpp"

Game::Game()
{
	cursor_and_keys_ = CursorAndKeys();

	Level menu = Level();
	levels_.push_back(menu);
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

	float button_w = 1280;
	float button_h = 720;
	float spacing = 50;
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
		levels_[0].AddLevelButton(i, spacing + button_w / 2 + (button_w + spacing) * c, spacing + button_h / 2 + (button_h + spacing) * r, button_w, button_h, identifier);

		c++;
		if (c == n_columns)
		{
			r++;
			c = 0;
		}
	}
	globals.active_level = 0;
}

void Game::Update(float dt)
{
	for (int i = 0; i < physics_ticks_per_frame_; ++i)
	{
		UpdatePhysics(dt / physics_ticks_per_frame_);
	}
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
			levels_[globals.active_level].SaveToFile("levels/level" + std::to_string(globals.active_level) + ".txt");
		}
		for (unsigned i = 1; i < levels_.size(); ++i)
		{
			levels_[i].LoadFromFile("levels/level" + std::to_string(i) + ".txt");
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
	}
	edit_mode_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	set_draw_info_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
}