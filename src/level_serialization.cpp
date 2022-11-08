#include "level.hpp"

static void SerializeComponent(Position c);
static void SerializeComponent(Charge c);
static void SerializeComponent(Velocity c);

static void DeserializeComponent(Position& c);
static void DeserializeComponent(Charge& c);
static void DeserializeComponent(Velocity& c);

void Level::SaveToFolder(std::string folder_path)
{
	std::map<int, Tag>& tags = GetComponent<Tag>();
	for (auto& [entity_id, tag_component] : tags)
	{
		std::string tag = tag_component.tag;
		if (tag == "Static-Particle")
		{
			SerializeComponent(GetComponent<Position>()[entity_id]);
			SerializeComponent(GetComponent<Charge>()[entity_id]);
		}
		if (tag == "Moving-Particle")
		{
			SerializeComponent(GetComponent<Position>()[entity_id]);
			SerializeComponent(GetComponent<Charge>()[entity_id]);
			SerializeComponent(GetComponent<Velocity>()[entity_id]);
		}
		if (tag == "Player")
		{
			SerializeComponent(GetComponent<Position>()[entity_id]);
			SerializeComponent(GetComponent<Charge>()[entity_id]);
			SerializeComponent(GetComponent<Velocity>()[entity_id]);
		}
	}
}

void Level::LoadFromFolder(std::string folder_path)
{
	for (auto& [_, component_map_variant] : components_)
	{
		std::visit([](auto& component_map) { component_map.clear(); }, component_map_variant);
	}

	std::vector<std::string> entity_tags_read_from_file;

	for (auto tag : entity_tags_read_from_file)
	{
		int entity_id = CreateEntityId();
		if (tag == "Static-Particle")
		{
			GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_red+.png" };
			GetComponent<Draggable>()[entity_id] = {};
			GetComponent<ClickedOn>()[entity_id] = {};

			DeserializeComponent(GetComponent<Position>()[entity_id]);
			DeserializeComponent(GetComponent<Charge>()[entity_id]);
		}
		if (tag == "Moving-Particle")
		{
			GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_blue+.png" };
			GetComponent<Draggable>()[entity_id] = {};
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<Acceleration>()[entity_id] = {};
			GetComponent<ReceivedForces>()[entity_id] = {};

			DeserializeComponent(GetComponent<Position>()[entity_id]);
			DeserializeComponent(GetComponent<Charge>()[entity_id]);
			DeserializeComponent(GetComponent<Velocity>()[entity_id]);
		}
		if (tag == "Player")
		{
			GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_blue+.png" };
			GetComponent<Draggable>()[entity_id] = {};
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<Acceleration>()[entity_id] = {};
			GetComponent<ReceivedForces>()[entity_id] = {};
			GetComponent<Player>()[entity_id] = {};

			DeserializeComponent(GetComponent<Position>()[entity_id]);
			DeserializeComponent(GetComponent<Charge>()[entity_id]);
			DeserializeComponent(GetComponent<Velocity>()[entity_id]);
		}
	}
}