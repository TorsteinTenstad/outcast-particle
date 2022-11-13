#include "game.hpp"

Game::Game()
{
	cursor_and_keys_ = CursorAndKeys();

	Level menu = Level();

	Level level1 = Level();
	level1.LoadFromFile("levels/level1.txt");

	Level level2 = Level();
	level2.LoadFromFile("levels/level2.txt");

	Level level3 = Level();
	level3.LoadFromFile("levels/level3.txt");

	levels_.push_back(menu);
	levels_.push_back(level1);
	levels_.push_back(level2);
	levels_.push_back(level3);
}

void Game::Init()
{
	sf::Texture texture;
	std::string identifier;
	for (unsigned i = 1; i < levels_.size(); ++i)
	{
		texture.create(globals.render_window.getSize().x, globals.render_window.getSize().y);
		globals.render_window.clear();
		globals.active_level = i;
		Update(0);
		texture.update(globals.render_window);
		identifier = "level" + std::to_string(i);
		render_system_.RegisterTexture(identifier, texture);
		levels_[0].AddLevelButton(i, 300, 300 + (i - 1) * (255), 400, 225, identifier);
	}
	globals.active_level = 0;
}

void Game::Update(float dt)
{
	event_system_.Update(cursor_and_keys_);

	if (cursor_and_keys_.key_pressed_this_frame[MENU_KEY] && globals.active_level != 0)
	{
		if (globals.edit_mode)
		{
			levels_[globals.active_level].SaveToFile("levels/level" + std::to_string(globals.active_level) + ".txt");
		}
		for (unsigned i = 1; i < levels_.size(); ++i)
		{
			levels_[i].LoadFromFile("levels/level" + std::to_string(i) + ".txt");
		}
		globals.edit_mode = false;
		globals.active_level = 0;
	}

	if (cursor_and_keys_.key_pressed_this_frame[EDIT_MODE_KEY] && globals.active_level != 0)
	{
		if (globals.edit_mode)
		{
			levels_[globals.active_level].SaveToFile("levels/level" + std::to_string(globals.active_level) + ".txt");
		}
		else
		{
			levels_[globals.active_level].LoadFromFile("levels/level" + std::to_string(globals.active_level) + ".txt");
		}
		globals.edit_mode = !globals.edit_mode;
	}
	mouse_interaction_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	level_button_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	if (!globals.edit_mode)
	{
		player_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		electric_force_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		electric_field_force_system.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		force_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		acceleration_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		velocity_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		intersection_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		goal_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		kill_on_intersection_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		collision_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	}
	else
	{
		edit_mode_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	}
	set_draw_info_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	render_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	if (globals.edit_mode)
	{
		display_velocity_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	}
}