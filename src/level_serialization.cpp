#include "level.hpp"
#include "string_parsing_utils.hpp"

void SerializeComponent(ExampleComponent c, std::string& str_rep);
void SerializeComponent(Position c, std::string& str_rep);
void SerializeComponent(Charge c, std::string& str_rep);
void SerializeComponent(Velocity c, std::string& str_rep);

void DeserializeComponent(Position& c, std::string str_rep);
void DeserializeComponent(Charge& c, std::string str_rep);
void DeserializeComponent(Velocity& c, std::string str_rep);

void SerializeComponent(ExampleComponent c, std::string& str_rep)
{
	str_rep += "ExampleComponent{";
	str_rep += "a=";
	str_rep += ToString(c.a);
	str_rep += ";b=";
	str_rep += ToString(c.b);
	str_rep += "}";
}
void SerializeComponent(Position c, std::string& str_rep)
{
	str_rep += "Position{";
	str_rep += "position=";
	str_rep += ToString(c.position);
	str_rep += "}";
}
void SerializeComponent(Charge c, std::string& str_rep)
{
	str_rep += "Charge{";
	str_rep += "charge=";
	str_rep += ToString(c.charge);
	str_rep += "}";
}
void SerializeComponent(Velocity c, std::string& str_rep)
{
	str_rep += "Velocity{";
	str_rep += "velocity=";
	str_rep += ToString(c.velocity);
	str_rep += "}";
}

void DeserializeComponent(ExampleComponent& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
		if (statement_parts[0] == "a")
		{
			FromString(c.a, statement_parts[1]);
		}
		if (statement_parts[0] == "b")
		{
			FromString(c.b, statement_parts[1]);
		}
	}
}

void DeserializeComponent(Position& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
		if (statement_parts[0] == "position")
		{
			FromString(c.position, statement_parts[1]);
		}
	}
}
void DeserializeComponent(Charge& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
		if (statement_parts[0] == "charge")
		{
			FromString(c.charge, statement_parts[1]);
		}
	}
}
void DeserializeComponent(Velocity& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
		if (statement_parts[0] == "velocity")
		{
			FromString(c.velocity, statement_parts[1]);
		}
	}
}

void Level::SaveToFile(std::string savefile_path)
{
	std::ofstream f(savefile_path);

	std::map<int, Tag>& tags = GetComponent<Tag>();
	std::string entity_string;
	for (auto& [entity_id, tag_component] : tags)
	{
		std::string tag = tag_component.tag;
		f << "\"" << tag << "\":";
		if (tag == "Static-Particle")
		{
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Charge>()[entity_id], entity_string);
		}
		if (tag == "Moving-Particle")
		{
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Charge>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Velocity>()[entity_id], entity_string);
		}
		if (tag == "Player")
		{
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Charge>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Velocity>()[entity_id], entity_string);
			SerializeComponent(GetComponent<ExampleComponent>()[entity_id], entity_string);
		}
		f << entity_string << "\n";
		entity_string.clear();
	}
}

void Level::LoadFromFile(std::string savefile_path)
{
	for (auto& [_, component_map_variant] : components_)
	{
		std::visit([](auto& component_map) { component_map.clear(); }, component_map_variant);
	}

	std::ifstream f(savefile_path);
	std::string line;
	while (getline(f, line))
	{
		int entity_id = CreateEntityId();
		std::string tag = GetSubstrBetween(line, "\"", "\"");
		if (tag == "Static-Particle")
		{
			GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_red+.png" };
			GetComponent<Draggable>()[entity_id] = {};
			GetComponent<ClickedOn>()[entity_id] = {};

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id], GetSubstrBetween(line, "Charge{", "}"));
		}
		if (tag == "Moving-Particle")
		{
			GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_green-.png" };
			GetComponent<Draggable>()[entity_id] = {};
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<Acceleration>()[entity_id] = {};
			GetComponent<ReceivedForces>()[entity_id] = {};

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id], GetSubstrBetween(line, "Charge{", "}"));
			DeserializeComponent(GetComponent<Velocity>()[entity_id], GetSubstrBetween(line, "Velocity{", "}"));
		}
		if (tag == "Player")
		{
			GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_blue+.png" };
			GetComponent<Draggable>()[entity_id] = {};
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<Acceleration>()[entity_id] = {};
			GetComponent<ReceivedForces>()[entity_id] = {};
			GetComponent<Player>()[entity_id] = {};

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id], GetSubstrBetween(line, "Charge{", "}"));
			DeserializeComponent(GetComponent<Velocity>()[entity_id], GetSubstrBetween(line, "Velocity{", "}"));
			DeserializeComponent(GetComponent<ExampleComponent>()[entity_id], GetSubstrBetween(line, "ExampleComponent{", "}"));
		}
	}
}