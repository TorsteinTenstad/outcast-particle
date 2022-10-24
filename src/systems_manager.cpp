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
	event_system_.Update(window, player_, received_forces_);
	electric_force_system_.Update(position_, charge_, received_forces_);
	force_system_.Update(acceleration_, received_forces_);
	acceleration_system_.Update(velocity_, acceleration_, dt);
	velocity_system_.Update(position_, velocity_, dt);
	render_system_.Update(window, draw_info_, position_);
}

int SystemsManager::CreateEntityId()
{
	return next_available_entity_id_++;
}

void SystemsManager::AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge)
{
	int id = CreateEntityId();
	if (charge > 0)
	{
		draw_info_[id] = { "content\\particle_100_blue+.png" };
	}
	else if (charge < 0)
	{
		draw_info_[id] = { "content\\particle_100_blue-.png" };
	}
	else
	{
		draw_info_[id] = { "content\\particle_100_blue.png" };
	}
	position_[id] = { sf::Vector2f(pos_x, pos_y) };
	velocity_[id] = { sf::Vector2f(vel_x, vel_y) };
	acceleration_[id] = { sf::Vector2f(0, 0) };
	charge_[id] = { charge };
	received_forces_[id] = ReceivedForces();
	player_[id] = Player();
}

void SystemsManager::AddParticleEntity(float pos_x, float pos_y, float charge)
{
	int id = CreateEntityId();
	if (charge > 0)
	{
		draw_info_[id] = { "content\\particle_100_red+.png" };
	}
	else if (charge < 0)
	{
		draw_info_[id] = { "content\\particle_100_green-.png" };
	}
	else
	{
		draw_info_[id] = { "content\\particle_100_blue-.png" };
	}
	position_[id] = { sf::Vector2f(pos_x, pos_y) };
	charge_[id] = { charge };
}

void SystemsManager::SaveEntitiesToFile(std::string path)
{
	(void)path;
}

void SystemsManager::LoadEntitiesFromFile(std::string path)
{
	(void)path;
}