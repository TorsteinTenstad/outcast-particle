#include "systems_manager.hpp"

SystemsManager::SystemsManager()
{
	sf::RectangleShape sprite;
	std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();
	texture->loadFromFile("content\\sfml.png");
	sprite.setTexture(std::move(texture));
	sprites_[0] = sprite;
}

void SystemsManager::Update(sf::RenderWindow& window, float dt)
{
	window.draw(sprites_[0]);
	(void)dt;
}