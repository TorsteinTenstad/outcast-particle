#pragma once
#include "PCH.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"

void SerializeComponent(ChargeDependentDrawInfo c, std::string& str_rep); //Is unused, but serves as an example of serialization of components with multiple members
void SerializeComponent(Position c, std::string& str_rep);
void SerializeComponent(Charge c, std::string& str_rep);
void SerializeComponent(Velocity c, std::string& str_rep);
void SerializeComponent(WidthAndHeight c, std::string& str_rep);

void DeserializeComponent(ChargeDependentDrawInfo& c, std::string str_rep); //Is unused, but serves as an example of deserialization of components with multiple members
void DeserializeComponent(Position& c, std::string str_rep);
void DeserializeComponent(Charge& c, std::string str_rep);
void DeserializeComponent(Velocity& c, std::string str_rep);
void DeserializeComponent(WidthAndHeight& c, std::string str_rep);

void SerializeComponent(ChargeDependentDrawInfo c, std::string& str_rep)
{
	str_rep += "ChargeDependentDrawInfo{";
	str_rep += "positive_charge_image_path=";
	str_rep += ToString(c.positive_charge_image_path);
	str_rep += ";neutral_charge_image_path=";
	str_rep += ToString(c.neutral_charge_image_path);
	str_rep += ";negative_charge_image_path=";
	str_rep += ToString(c.negative_charge_image_path);
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
void SerializeComponent(WidthAndHeight c, std::string& str_rep)
{
	str_rep += "WidthAndHeight{";
	str_rep += "width_and_height=";
	str_rep += ToString(c.width_and_height);
	str_rep += "}";
}

void DeserializeComponent(ChargeDependentDrawInfo& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
		if (statement_parts[0] == "positive_charge_image_path")
		{
			FromString(c.positive_charge_image_path, statement_parts[1]);
		}
		if (statement_parts[0] == "neutral_charge_image_path")
		{
			FromString(c.neutral_charge_image_path, statement_parts[1]);
		}
		if (statement_parts[0] == "negative_charge_image_path")
		{
			FromString(c.negative_charge_image_path, statement_parts[1]);
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
void DeserializeComponent(WidthAndHeight& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
		if (statement_parts[0] == "width_and_height")
		{
			FromString(c.width_and_height, statement_parts[1]);
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
		}
		if (tag == "Wall")
		{
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
		}
		if (tag == "Gloal")
		{
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
		}
		if (tag == "Laser")
		{
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
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
		GetComponent<Tag>()[entity_id].tag = tag;
		if (tag == "Static-Particle")
		{
			GetComponent<ChargeDependentDrawInfo>()[entity_id] = { "content\\particle_100_red+.png", "content\\particle_100_green.png", "content\\particle_100_green-.png" };
			GetComponent<Editable>()[entity_id] = { true, true, false, sf::Vector2f(0, 0), false };
			GetComponent<ClickedOn>()[entity_id];
			GetComponent<Radius>()[entity_id] = { 50 };

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id], GetSubstrBetween(line, "Charge{", "}"));
		}
		if (tag == "Moving-Particle")
		{
			GetComponent<ChargeDependentDrawInfo>()[entity_id] = { "content\\particle_100_red+.png", "content\\particle_100_green.png", "content\\particle_100_green-.png" };
			GetComponent<Editable>()[entity_id] = { true, true, false, sf::Vector2f(0, 0), false };
			GetComponent<ClickedOn>()[entity_id];
			GetComponent<Acceleration>()[entity_id];
			GetComponent<ReceivedForces>()[entity_id];
			GetComponent<Radius>()[entity_id] = { 50 };

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id], GetSubstrBetween(line, "Charge{", "}"));
			DeserializeComponent(GetComponent<Velocity>()[entity_id], GetSubstrBetween(line, "Velocity{", "}"));
		}
		if (tag == "Player")
		{
			GetComponent<ChargeDependentDrawInfo>()[entity_id] = { "content\\particle_100_blue+.png", "content\\particle_100_blue.png", "content\\particle_100_blue-.png" };
			GetComponent<Editable>()[entity_id] = { true, true, false, sf::Vector2f(0, 0), false };
			GetComponent<ClickedOn>()[entity_id];
			GetComponent<Acceleration>()[entity_id];
			GetComponent<ReceivedForces>()[entity_id];
			GetComponent<Player>()[entity_id];
			GetComponent<Radius>()[entity_id] = { 50 };

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id], GetSubstrBetween(line, "Charge{", "}"));
			DeserializeComponent(GetComponent<Velocity>()[entity_id], GetSubstrBetween(line, "Velocity{", "}"));
		}
		if (tag == "Wall")
		{
			GetComponent<DrawInfo>()[entity_id] = { "content\\block.png" };
			GetComponent<Editable>()[entity_id] = { false, false, true, sf::Vector2f(0, 0), false };
			GetComponent<ClickedOn>()[entity_id];

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id], GetSubstrBetween(line, "WidthAndHeight{", "}"));
		}
		if (tag == "Gloal")
		{
			GetComponent<DrawInfo>()[entity_id] = { "content\\goal.png" };
			GetComponent<Editable>()[entity_id] = { false, false, true, sf::Vector2f(0, 0), false };
			GetComponent<ClickedOn>()[entity_id];
			GetComponent<Goal>()[entity_id];
			GetComponent<KillOnIntersection>()[entity_id];

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id], GetSubstrBetween(line, "WidthAndHeight{", "}"));
		}
		if (tag == "Laser")
		{
			GetComponent<OrientationDependentDrawInfo>()[entity_id] = { "content\\laser_horisontal.png", "content\\laser_vertical.png" };
			GetComponent<Editable>()[entity_id] = { false, false, true, sf::Vector2f(0, 0), false };
			GetComponent<ClickedOn>()[entity_id];
			GetComponent<Goal>()[entity_id];
			GetComponent<KillOnIntersection>()[entity_id];

			DeserializeComponent(GetComponent<Position>()[entity_id], GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id], GetSubstrBetween(line, "WidthAndHeight{", "}"));
		}
	}
}