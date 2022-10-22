#include "systems_manager.hpp"

SystemsManager::SystemsManager()
{
}

void SystemsManager::Update(sf::RenderWindow& window, float dt)
{
	render_system_.Update(draw_info_)
}

void SystemsManager::AddEntity()
{
	DrawInfo d;
	d.image_path = "content\\sfml.png";
	draw_info_[1] = d;
}