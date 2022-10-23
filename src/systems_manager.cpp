#include "systems_manager.hpp"

SystemsManager::SystemsManager()
{
	event_system_ = EventSystem();
	acceleration_system_ = AccelerationSystem();
	velocity_system_ = VelocitySystem();
	render_system_ = RenderSystem();
	electric_force_system_ = ElectricForceSystem();
	force_system_ = ForceSystem();
}

void SystemsManager::Update(sf::RenderWindow& window, float dt)
{
	event_system_.Update(window, affected_by_keyboard_forces_, received_forces_);
	electric_force_system_.Update(position_, charge_, received_forces_);
	force_system_.Update(acceleration_, received_forces_);
	acceleration_system_.Update(velocity_, acceleration_, dt);
	velocity_system_.Update(position_, velocity_, dt);
	render_system_.Update(window, draw_info_, position_);
}

void SystemsManager::AddEntity()
{
	DrawInfo d;
	d.image_path = "content\\particle_100_blue-.png";
	draw_info_[1] = d;
	Position p;
	p.position = sf::Vector2f(400, 350);
	position_[1] = p;
	Velocity v;
	v.velocity = sf::Vector2f(0, 500);
	velocity_[1] = v;
	Acceleration a;
	a.acceleration = sf::Vector2f(0, 0);
	acceleration_[1] = a;
	Charge c;
	c.charge = -10000;
	charge_[1] = c;
	ReceivedForces rf;
	rf.mass = 1;
	received_forces_[1] = rf;
	AffectedByKeyboardForces abkf;
	affected_by_keyboard_forces_[1] = abkf;

	DrawInfo d2;
	d2.image_path = "content\\particle_100_red+.png";
	draw_info_[2] = d2;
	Position p2;
	p2.position = sf::Vector2f(825, 350);
	position_[2] = p2;
	Charge c2;
	c2.charge = 10000;
	charge_[2] = c2;
}