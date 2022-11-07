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

int Level::CreateEntityId()
{
	return next_available_entity_id_++;
}

int Level::AddParticleEntity(float pos_x, float pos_y, float charge)
{
	int id = CreateEntityId();
	if (charge > 0)
	{
		GetComponent<DrawInfo>()[id] = { "content\\particle_100_red+.png" };
	}
	else if (charge < 0)
	{
		GetComponent<DrawInfo>()[id] = { "content\\particle_100_green-.png" };
	}
	else
	{
		GetComponent<DrawInfo>()[id] = { "" };
	}
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<Charge>()[id] = { charge };
	GetComponent<Draggable>()[id] = {};
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<Radius>()[id] = { 50 };
	return id;
}

int Level::AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge)
{
	int id = AddParticleEntity(pos_x, pos_y, charge);
	GetComponent<Velocity>()[id] = { sf::Vector2f(vel_x, vel_y) };
	GetComponent<Acceleration>()[id] = { sf::Vector2f(0, 0) };
	GetComponent<ReceivedForces>()[id] = ReceivedForces();
	return id;
}

int Level::AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge)
{
	int id = AddMovingParticleEntity(pos_x, pos_y, vel_x, vel_y, charge);
	if (charge > 0)
	{
		GetComponent<DrawInfo>()[id] = { "content\\particle_100_blue+.png" };
	}
	else if (charge < 0)
	{
		GetComponent<DrawInfo>()[id] = { "content\\particle_100_blue-.png" };
	}
	else
	{
		GetComponent<DrawInfo>()[id] = { "content\\particle_100_blue.png" };
	}
	GetComponent<Player>()[id] = Player();
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

int Level::AddLaser()
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id] = { "content\\laser.png" };
	GetComponent<Position>()[id] = { sf::Vector2f(1000, 0) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(48, 480) };
	GetComponent<Draggable>()[id] = {};
	GetComponent<ClickedOn>()[id] = {};
	return id;
}

int Level::AddBlock(float pos_x, float pos_y)
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id] = { "content\\block.png" };
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(48, 48) };
	GetComponent<Draggable>()[id] = {};
	GetComponent<ClickedOn>()[id] = {};
	return id;
}