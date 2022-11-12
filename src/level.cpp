#include "level.hpp"

int Level::next_available_entity_id_ = 0;

template <class Component>
void Level::RegisterComponent()
{
	std::map<int, Component> t_map;
	components_[typeid(Component)] = t_map;
}

template <class Component>
std::map<int, Component>& Level::GetComponent()
{
	if (components_.count(typeid(Component)) == 0)
	{
		RegisterComponent<Component>();
	}
	return std::get<std::map<int, Component>>(components_[typeid(Component)]);
}

int Level::CopyEntity(int from_id)
{
	int to_id = CreateEntityId();
	for (auto& [_, component_map_variant] : components_)
	{
		(void)_;
		std::visit([from_id, to_id](auto& component_map) {
			if (component_map.count(from_id) != 0)
			{
				component_map[to_id] = component_map[from_id];
			}
		},
			component_map_variant);
	}
	return to_id;
}

void Level::DeleteEntity(int id)
{
	for (auto& [_, component_map_variant] : components_)
	{
		(void)_;
		std::visit([id](auto& component_map) {
			if (component_map.count(id) != 0)
			{
				component_map.erase(id);
			}
		},
			component_map_variant);
	}
}

int Level::CreateEntityId()
{
	return next_available_entity_id_++;
}

int Level::AddParticleEntity(float pos_x, float pos_y, float charge)
{
	int id = CreateEntityId();
	GetComponent<ChargeDependentDrawInfo>()[id] = { "content\\particle_100_red+.png", "content\\particle_100_red.png", "content\\particle_100_green-.png" };
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<Charge>()[id] = { charge };
	GetComponent<Editable>()[id].is_charge_editable = true;
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<Radius>()[id] = { 50 };
	return id;
}

int Level::AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge)
{
	int id = AddParticleEntity(pos_x, pos_y, charge);
	GetComponent<Editable>()[id].is_velocity_editable = true;
	GetComponent<Velocity>()[id] = { sf::Vector2f(vel_x, vel_y) };
	GetComponent<Acceleration>()[id] = { sf::Vector2f(0, 0) };
	GetComponent<ReceivedForces>()[id] = ReceivedForces();
	return id;
}

int Level::AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge, float player_force)
{
	int id = AddMovingParticleEntity(pos_x, pos_y, vel_x, vel_y, charge);
	GetComponent<ChargeDependentDrawInfo>()[id] = { "content\\particle_100_blue+.png", "content\\particle_100_blue.png", "content\\particle_100_blue-.png" };
	GetComponent<Player>()[id].move_force = player_force;
	GetComponent<Intersection>()[id] = {};
	return id;
}

int Level::AddLevelButton(int level, float pos_x, float pos_y, float width, float height, std::string path)
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id] = { path };
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(width, height) };
	GetComponent<Border>()[id] = { 5, sf::Color::White };
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<LevelButton>()[id] = { level };
	return id;
}

int Level::AddLaser(float pos_x, float pos_y, float width, float height)
{
	int id = CreateEntityId();
	GetComponent<OrientationDependentDrawInfo>()[id] = { "content\\laser_horisontal.png", "content\\laser_vertical.png" };
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(width, height) };
	GetComponent<Editable>()[id].is_height_and_widht_editable = true;
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<KillOnIntersection>()[id] = {};
	return id;
}

int Level::AddBlock(float pos_x, float pos_y)
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id] = { "content\\block.png" };
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(48, 48) };
	GetComponent<Editable>()[id].is_height_and_widht_editable = true;
	GetComponent<ClickedOn>()[id] = {};
	return id;
}

int Level::AddGoal()
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id] = { "content\\goal.png" };
	GetComponent<Position>()[id] = { sf::Vector2f(1500, 500) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(96, 96) };
	GetComponent<Editable>()[id].is_height_and_widht_editable = true;
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<Goal>()[id] = {};
	GetComponent<KillOnIntersection>()[id] = {};
	return id;
}