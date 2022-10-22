#include "systems_manager.hpp"

SystemsManager::SystemsManager()
{
	render_system_ = RenderSystem();
}

void SystemsManager::Update(sf::RenderWindow& window, float dt)
{
	(void)dt;
	render_system_.Update(window, draw_info_, position_);
}

void SystemsManager::AddEntity()
{
	DrawInfo d;
	d.image_path = "content\\sfml.png";
	draw_info_[1] = d;
	Position p;
	p.position = sf::Vector2f(825, 350);
	position_[1] = p;
}