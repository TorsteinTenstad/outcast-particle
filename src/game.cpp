#include "game.hpp"

Game::Game()
{
	Level level1 = Level();
	level1.AddPlayerEntity(1920, 1080, 0, 0, -100);
	level1.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 + 300, -100, 100, -100);
	level1.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 - 300, 100, 100, -100);
	level1.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 + 300, -100, -100, -100);
	level1.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 - 300, 100, -100, -100);
	level1.AddParticleEntity(1920 / 2, 1080 / 2, 1000000);

	Level level2 = Level();
	level2.AddPlayerEntity(1920, 1080, 0, 0, -100);
	level2.AddParticleEntity(1920 / 2, 1080 / 2, 1000000);

	levels_.push_back(level1);
	levels_.push_back(level2);
}

void Game::Update(float dt)
{
	levels_[globals.active_level].Update(dt);
}