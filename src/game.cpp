#include "game.hpp"

Game::Game()
{
	cursor_and_keys_ = CursorAndKeys();

	Level level1 = Level();
	level1.AddPlayerEntity(1920, 1080, 0, 0, -100);
	level1.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 + 300, -100, 100, -100);
	level1.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 - 300, 100, 100, -100);
	level1.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 + 300, -100, -100, -100);
	level1.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 - 300, 100, -100, -100);
	level1.AddParticleEntity(1920 / 2, 1080 / 2, 1000000);

	Level level2 = Level();
	level2.AddPlayerEntity(1920 / 2, 1080 / 2, 0, 0, -100);

	levels_.push_back(level1);
	levels_.push_back(level2);
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
	levels_[globals.active_level].Update(cursor_and_keys_, dt);
}