#include "systems_manager.hpp"

SystemsManager::SystemsManager()
{
	render_system_ = RenderSystem();
	velocity_system_ = VelocitySystem();
}

void SystemsManager::Update(sf::RenderWindow& window, float dt)
{
	velocity_system_.Update(position_, velocity_, dt);
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
	Velocity v;
	v.velocity = sf::Vector2f(50, 0);
	velocity_[1] = v;
}