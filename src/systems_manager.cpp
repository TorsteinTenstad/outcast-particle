#include "systems_manager.hpp"

SystemsManager::SystemsManager()
{
	render_system_ = RenderSystem();
	velocity_system_ = VelocitySystem();
	acceleration_system_ = AccelerationSystem();
}

void SystemsManager::Update(sf::RenderWindow& window, float dt)
{
	acceleration_system_.Update(velocity_, acceleration_, dt);
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
	v.velocity = sf::Vector2f(900, 0);
	velocity_[1] = v;
	Acceleration a;
	a.acceleration = sf::Vector2f(-500, 0);
	acceleration_[1] = a;
}