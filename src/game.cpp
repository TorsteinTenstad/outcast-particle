#include "game.hpp"

Game::Game()
{
	Level level = Level();
	level.AddPlayerEntity(1920, 1080, 0, 0, -100);
	level.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 + 300, -100, 100, -100);
	level.AddMovingParticleEntity(1920 / 2 + 300, 1080 / 2 - 300, 100, 100, -100);
	level.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 + 300, -100, -100, -100);
	level.AddMovingParticleEntity(1920 / 2 - 300, 1080 / 2 - 300, 100, -100, -100);
	level.AddParticleEntity(1920 / 2, 1080 / 2, 1000000);
	levels_.push_back(level);
}

void Game::Update(sf::RenderWindow& window, float dt)
{
	levels_[0].Update(window, dt);
}