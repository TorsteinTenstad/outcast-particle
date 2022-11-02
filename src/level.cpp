#include "level.hpp"

int Level::CreateEntityId()
{
	return globals.next_available_entity_id++;
}

int Level::CopyEntity(int from_id)
{
	int id = CreateEntityId();
	if (draw_info_.count(from_id))
	{
		draw_info_[id] = draw_info_[from_id];
	}
	if (position_.count(from_id))
	{
		position_[id] = position_[from_id];
	}
	if (velocity_.count(from_id))
	{
		velocity_[id] = velocity_[from_id];
	}
	if (acceleration_.count(from_id))
	{
		acceleration_[id] = acceleration_[from_id];
	}
	if (received_forces_.count(from_id))
	{
		received_forces_[id] = received_forces_[from_id];
	}
	if (player_.count(from_id))
	{
		player_[id] = player_[from_id];
	}
	if (charge_.count(from_id))
	{
		charge_[id] = charge_[from_id];
	}
	if (draggable_.count(from_id))
	{
		draggable_[id] = draggable_[from_id];
	}
	if (clicked_on_.count(from_id))
	{
		clicked_on_[id] = clicked_on_[from_id];
	}
	if (radius_.count(from_id))
	{
		radius_[id] = radius_[from_id];
	}
	if (width_and_hight_.count(from_id))
	{
		width_and_hight_[id] = width_and_hight_[from_id];
	}
	if (boarder_.count(from_id))
	{
		boarder_[id] = boarder_[from_id];
	}
	if (level_button_.count(from_id))
	{
		level_button_[id] = level_button_[from_id];
	}
	return id;
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
	width_and_hight_[id] = { sf::Vector2f(48, 480) };
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