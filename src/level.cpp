#include "level.hpp"

int Level::next_available_entity_id_ = 0;

Level::Level()
{
	GetComponent<DrawInfo>();
	GetComponent<ChargeDependentDrawInfo>();
	GetComponent<OrientationDependentDrawInfo>();
	GetComponent<Position>();
	GetComponent<Velocity>();
	GetComponent<Acceleration>();
	GetComponent<ReceivedForces>();
	GetComponent<BlueprintMenuItem>();
	GetComponent<Player>();
	GetComponent<PlayerBehaviours>();
	GetComponent<Collision>();
	GetComponent<Charge>();
	GetComponent<Editable>();
	GetComponent<ClickedOn>();
	GetComponent<Radius>();
	GetComponent<WidthAndHeight>();
	GetComponent<Border>();
	GetComponent<Trail>();
	GetComponent<Tag>();
	GetComponent<KillOnIntersection>();
	GetComponent<Goal>();
	GetComponent<ElectricField>();
	GetComponent<MagneticField>();
	GetComponent<Intersection>();
	GetComponent<LevelButton>();
	GetComponent<SoundInfo>();
}

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

void Level::LoadFromFile()
{
	LoadFromFile(savefile_path_);
}

void Level::SaveToFile()
{
	SaveToFile(savefile_path_);
}

int Level::AddLevelButton(int level, float pos_x, float pos_y, float width, float height, std::string path)
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id].image_path = path;
	GetComponent<DrawInfo>()[id].scale_to_fit = true;
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(width, height) };
	GetComponent<Border>()[id] = { 20, sf::Color::White };
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<LevelButton>()[id] = { level };
	return id;
}