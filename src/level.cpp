#include "level.hpp"

Level::Level()
{
	event_system_ = SFMLEventSystem();
	cursor_interaction_system_ = CursorInteractionSystem();
	keyboard_force_system_ = KeyboardForceSystem();
	electric_force_system_ = ElectricForceSystem();
	force_system_ = ForceSystem();
	acceleration_system_ = AccelerationSystem();
	velocity_system_ = VelocitySystem();
	render_system_ = SFMLRenderSystem();
}

void Level::Update(float dt)
{
	event_system_.Update(cursor_, player_);
	keyboard_force_system_.Update(player_, received_forces_);
	electric_force_system_.Update(position_, charge_, received_forces_);
	force_system_.Update(acceleration_, received_forces_);
	acceleration_system_.Update(velocity_, acceleration_, dt);
	velocity_system_.Update(position_, velocity_, dt);
	cursor_interaction_system_.Update(cursor_, draggable_, radius_, clicked_on_, position_);
	render_system_.Update(draw_info_, position_);
}

int Level::CreateEntityId()
{
	return next_available_entity_id_++;
}

void Level::AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge)
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

void Level::AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge)
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
		draw_info_[id] = { "content\\particle_100_blue.png" };
	}
	position_[id] = { sf::Vector2f(pos_x, pos_y) };
	velocity_[id] = { sf::Vector2f(vel_x, vel_y) };
	acceleration_[id] = { sf::Vector2f(0, 0) };
	charge_[id] = { charge };
	received_forces_[id] = ReceivedForces();
}

void Level::AddParticleEntity(float pos_x, float pos_y, float charge)
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
		draw_info_[id] = { "" };
	}
	position_[id] = { sf::Vector2f(pos_x, pos_y) };
	charge_[id] = { charge };
	draggable_[id] = {};
	radius_[id] = { 50 };
}

void Level::SaveEntitiesToFile(std::string path)
{
	(void)path;
}

void Level::LoadEntitiesFromFile(std::string path)
{
	(void)path;
}