#include "level.hpp"

int Level::CreateEntityId()
{
	return globals.next_available_entity_id++;
}

int Level::AddParticleEntity(float pos_x, float pos_y, float charge)
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
	clicked_on_[id] = {};
	radius_[id] = { 50 };
	return id;
}

int Level::AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge)
{
	int id = AddParticleEntity(pos_x, pos_y, charge);
	velocity_[id] = { sf::Vector2f(vel_x, vel_y) };
	acceleration_[id] = { sf::Vector2f(0, 0) };
	received_forces_[id] = ReceivedForces();
	return id;
}

int Level::AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge)
{
	int id = AddMovingParticleEntity(pos_x, pos_y, vel_x, vel_y, charge);
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
	player_[id] = Player();
	return id;
}

void Level::SaveEntitiesToFile(std::string path)
{
	(void)path;
}

void Level::LoadEntitiesFromFile(std::string path)
{
	(void)path;
}

int Level::AddLevelButton(int level, float pos_x, float pos_y, float width, float hight, std::string path)
{
	int id = CreateEntityId();
	draw_info_[id] = { path };
	position_[id] = { sf::Vector2f(pos_x, pos_y) };
	width_and_hight_[id] = { sf::Vector2f(width, hight) };
	boarder_[id] = { 5, sf::Color::White };
	clicked_on_[id] = {};
	level_button_[id] = { level };
	return id;
}

int Level::AddLaser()
{
	int id = CreateEntityId();
	draw_info_[id] = { "content\\laser.png" };
	position_[id] = { sf::Vector2f(1000, 0) };
	width_and_hight_[id] = { sf::Vector2f(50, 1000) };
	draggable_[id] = {};
	clicked_on_[id] = {};
	return id;
}

int Level::AddBlock(float pos_x, float pos_y)
{
	int id = CreateEntityId();
	draw_info_[id] = { "content\\block.png" };
	position_[id] = { sf::Vector2f(pos_x, pos_y) };
	width_and_hight_[id] = { sf::Vector2f(48, 48) };
	draggable_[id] = {};
	clicked_on_[id] = {};
	return id;
}