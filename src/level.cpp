#include "level.hpp"

int Level::next_available_entity_id_ = 1;

template <class Component>
void Level::RegisterComponent()
{
	std::map<int, Component> component_map;
	components_[typeid(Component).name()] = component_map;
}

template <class Component>
std::map<int, Component>& Level::GetComponent()
{
	if (components_.count(typeid(Component).name()) == 0)
	{
		RegisterComponent<Component>();
	}
	return std::get<std::map<int, Component>>(components_[typeid(Component).name()]);
}

void Level::SaveToFolder(std::string folder_path) const
{
	for (auto& [component_typeid_name, component_map_variant] : components_)
	{
		std::visit([folder_path, component_typeid_name](auto& component_map) {
			for (const auto& [entity_id, component] : component_map)
			{
				std::string unique_id = folder_path + std::to_string(entity_id) + "-" + std::string(component_typeid_name);
				SaveToBinaryFile(component, unique_id.c_str());
			}
		},
			component_map_variant);
	}
}

void Level::LoadFromFolder(std::string folder_path)
{
	//for (auto& [_, component_map_variant] : components_)
	//{
	//	(void)_;
	//		std::visit([](auto& component_map) { component_map.clear(); }, component_map_variant);
	//	}
	for (const auto& entry : std::experimental::filesystem::directory_iterator(folder_path))
	{
		std::string filename = entry.path().filename().string();
		size_t separator_index = filename.find("-");
		int entity_id = stoi(filename.substr(0, separator_index));
		std::string component_typeid_name = filename.substr(separator_index + 1, filename.length());
		std::cout << entity_id << "," << component_typeid_name << "\n";
		std::visit([entry, entity_id, component_typeid_name](auto& component_map) {
			component_map[entity_id];
			LoadFromBinaryFile(component_map[entity_id], entry.path().string().c_str());
		},
			components_[component_typeid_name.c_str()]);
	}
}

int Level::CreateEntityId()
{
	return next_available_entity_id_++;
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

int Level::AddLevelButton(int level, float pos_x, float pos_y, float width, float hight, std::string path)
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id] = { path };
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHight>()[id] = { sf::Vector2f(width, hight) };
	GetComponent<Boarder>()[id] = { 5, sf::Color::White };
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<LevelButton>()[id] = { level };
	return id;
}

int Level::AddLaser()
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id] = { "content\\laser.png" };
	GetComponent<Position>()[id] = { sf::Vector2f(1000, 0) };
	GetComponent<WidthAndHight>()[id] = { sf::Vector2f(48, 480) };
	GetComponent<Draggable>()[id] = {};
	GetComponent<ClickedOn>()[id] = {};
	return id;
}

int Level::AddBlock(float pos_x, float pos_y)
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id] = { "content\\block.png" };
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHight>()[id] = { sf::Vector2f(48, 48) };
	GetComponent<Draggable>()[id] = {};
	GetComponent<ClickedOn>()[id] = {};
	return id;
}