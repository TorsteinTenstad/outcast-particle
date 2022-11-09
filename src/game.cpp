#include "game.hpp"

Game::Game()
{
	cursor_and_keys_ = CursorAndKeys();

	Level menu = Level();

	Level level1 = Level();
	level1.AddPlayerEntity(1920, 1080, 0, 0, -100, 3000);
	level1.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 + 300, -100, 100, -100);
	level1.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 - 300, 100, 100, -100);
	level1.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 + 300, -100, -100, -100);
	level1.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 - 300, 100, -100, -100);
	level1.AddParticleEntity(1920 / 2, 1080 / 2, 1000000);

	Level level2 = Level();
	level2.AddPlayerEntity(1920 / 10, 1080 / 2, 0, 0, -100, 100);
	level2.AddLaser(1920 / 2, 24, 1920, 48);
	level2.AddLaser(24, 1080 / 2, 48, 1080);
	level2.AddLaser(1920 / 2, 1080 - 24, 1920, 48);
	level2.AddLaser(1920 - 24, 1080 / 2, 48, 1080);
	level2.AddGoal();
	//level2.AddBlock(0, 0);
	//level2.AddBlock(48, 0);
	//level2.AddBlock(96, 0);
	//level2.AddBlock(200, 200);
	level2.AddParticleEntity(1920 / 5, 1080 / 10, 1000000);
	level2.AddParticleEntity(1920 / 5, 1080 / 10 * 9, 1000000);

	levels_.push_back(menu);
	levels_.push_back(level1);
	levels_.push_back(level2);

	sf::Texture texture;
	texture.create(globals.render_window.getSize().x, globals.render_window.getSize().y);

	globals.render_window.clear();
	globals.active_level = 1;
	Update(0);
	texture.update(globals.render_window);
	render_system_.RegisterTexture("level1", texture);
	levels_[0].AddLevelButton(1, 300, 300, 400, 225, "level1");

	globals.render_window.clear();
	globals.active_level = 2;
	Update(0);
	texture.update(globals.render_window);
	render_system_.RegisterTexture("level2", texture);
	levels_[0].AddLevelButton(2, 300, 625, 400, 225, "level2");

	globals.active_level = 0;
}

void Game::InitSystems()
{
	for (auto& level : levels_)
	{
		player_system_.Init(level);
	}
}

void Game::Update(float dt)
{
	event_system_.Update(cursor_and_keys_);

	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape])
	{
		globals.edit_mode = !globals.edit_mode;
	}
	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Num0])
	{
		globals.active_level = 0;
	}
	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Num1])
	{
		globals.active_level = 1;
	}
	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Num2])
	{
		globals.active_level = 2;
	}

	mouse_interaction_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	level_button_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	if (!globals.edit_mode)
	{
		player_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		electric_force_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		force_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		acceleration_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		velocity_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		intersection_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		goal_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
		kill_on_intersection_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	}
	else
	{
		edit_mode_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	}
	render_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	if (globals.edit_mode)
	{
		display_velocity_system_.Update(cursor_and_keys_, levels_[globals.active_level], dt);
	}
}