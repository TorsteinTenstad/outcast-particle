
#pragma once
#include "PCH.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"
/*
Warning!
Code is generated.
Changes will be overwritten.
*/


void SerializeComponent(Radius c, std::string& str_rep)
{
	str_rep += "Radius{";
	str_rep += "radius=";
	str_rep += ToString(c.radius);
	str_rep += "}";
}

void DeserializeComponent(Radius& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "radius")
		{
			FromString(c.radius, statement_parts[1]);
		}
	}
}

void SerializeComponent(WidthAndHeight c, std::string& str_rep)
{
	str_rep += "WidthAndHeight{";
	str_rep += "width_and_height=";
	str_rep += ToString(c.width_and_height);
	str_rep += "}";
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

void SerializeComponent(Border c, std::string& str_rep)
{
	str_rep += "Border{";
	str_rep += "thickness=";
	str_rep += ToString(c.thickness);
	str_rep += ";";
	str_rep += "color=";
	str_rep += ToString(c.color);
	str_rep += "}";
}

void DeserializeComponent(Border& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "thickness")
		{
			FromString(c.thickness, statement_parts[1]);
		}

		if (statement_parts[0] == "color")
		{
			FromString(c.color, statement_parts[1]);
		}
	}
}

void SerializeComponent(DrawInfo c, std::string& str_rep)
{
	str_rep += "DrawInfo{";
	str_rep += "scale_to_fit=";
	str_rep += ToString(c.scale_to_fit);
	str_rep += ";";
	str_rep += "image_path=";
	str_rep += ToString(c.image_path);
	str_rep += "}";
}

void DeserializeComponent(DrawInfo& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "scale_to_fit")
		{
			FromString(c.scale_to_fit, statement_parts[1]);
		}

		if (statement_parts[0] == "image_path")
		{
			FromString(c.image_path, statement_parts[1]);
		}
	}
}

void SerializeComponent(ChargeDependentDrawInfo c, std::string& str_rep)
{
	str_rep += "ChargeDependentDrawInfo{";
	str_rep += "positive_charge_image_path=";
	str_rep += ToString(c.positive_charge_image_path);
	str_rep += ";";
	str_rep += "neutral_charge_image_path=";
	str_rep += ToString(c.neutral_charge_image_path);
	str_rep += ";";
	str_rep += "negative_charge_image_path=";
	str_rep += ToString(c.negative_charge_image_path);
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

void SerializeComponent(OrientationDependentDrawInfo c, std::string& str_rep)
{
	str_rep += "OrientationDependentDrawInfo{";
	str_rep += "horisontal_image_path=";
	str_rep += ToString(c.horisontal_image_path);
	str_rep += ";";
	str_rep += "vertical_image_path=";
	str_rep += ToString(c.vertical_image_path);
	str_rep += "}";
}

void DeserializeComponent(OrientationDependentDrawInfo& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "horisontal_image_path")
		{
			FromString(c.horisontal_image_path, statement_parts[1]);
		}

		if (statement_parts[0] == "vertical_image_path")
		{
			FromString(c.vertical_image_path, statement_parts[1]);
		}
	}
}

void SerializeComponent(Goal c, std::string& str_rep)
{
	str_rep += "Goal{";
	str_rep += "is_goal=";
	str_rep += ToString(c.is_goal);
	str_rep += "}";
}

void DeserializeComponent(Goal& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "is_goal")
		{
			FromString(c.is_goal, statement_parts[1]);
		}
	}
}

void SerializeComponent(Intersection c, std::string& str_rep)
{
	str_rep += "Intersection{";
	str_rep += "}";
}

void DeserializeComponent(Intersection& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
	}
}

void SerializeComponent(KillOnIntersection c, std::string& str_rep)
{
	str_rep += "KillOnIntersection{";
	str_rep += "}";
}

void DeserializeComponent(KillOnIntersection& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
	}
}

void SerializeComponent(LevelButton c, std::string& str_rep)
{
	str_rep += "LevelButton{";
	str_rep += "level=";
	str_rep += ToString(c.level);
	str_rep += "}";
}

void DeserializeComponent(LevelButton& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "level")
		{
			FromString(c.level, statement_parts[1]);
		}
	}
}

void SerializeComponent(Editable c, std::string& str_rep)
{
	str_rep += "Editable{";
	str_rep += "is_charge_editable=";
	str_rep += ToString(c.is_charge_editable);
	str_rep += ";";
	str_rep += "is_velocity_editable=";
	str_rep += ToString(c.is_velocity_editable);
	str_rep += ";";
	str_rep += "is_height_and_widht_editable=";
	str_rep += ToString(c.is_height_and_widht_editable);
	str_rep += ";";
	str_rep += "selected=";
	str_rep += ToString(c.selected);
	str_rep += ";";
	str_rep += "drag_offset=";
	str_rep += ToString(c.drag_offset);
	str_rep += "}";
}

void DeserializeComponent(Editable& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "is_charge_editable")
		{
			FromString(c.is_charge_editable, statement_parts[1]);
		}

		if (statement_parts[0] == "is_velocity_editable")
		{
			FromString(c.is_velocity_editable, statement_parts[1]);
		}

		if (statement_parts[0] == "is_height_and_widht_editable")
		{
			FromString(c.is_height_and_widht_editable, statement_parts[1]);
		}

		if (statement_parts[0] == "selected")
		{
			FromString(c.selected, statement_parts[1]);
		}

		if (statement_parts[0] == "drag_offset")
		{
			FromString(c.drag_offset, statement_parts[1]);
		}
	}
}

void SerializeComponent(ClickedOn c, std::string& str_rep)
{
	str_rep += "ClickedOn{";
	str_rep += "clicked_on=";
	str_rep += ToString(c.clicked_on);
	str_rep += ";";
	str_rep += "clicked_this_frame=";
	str_rep += ToString(c.clicked_this_frame);
	str_rep += ";";
	str_rep += "released_this_frame=";
	str_rep += ToString(c.released_this_frame);
	str_rep += "}";
}

void DeserializeComponent(ClickedOn& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "clicked_on")
		{
			FromString(c.clicked_on, statement_parts[1]);
		}

		if (statement_parts[0] == "clicked_this_frame")
		{
			FromString(c.clicked_this_frame, statement_parts[1]);
		}

		if (statement_parts[0] == "released_this_frame")
		{
			FromString(c.released_this_frame, statement_parts[1]);
		}
	}
}

void SerializeComponent(Position c, std::string& str_rep)
{
	str_rep += "Position{";
	str_rep += "position=";
	str_rep += ToString(c.position);
	str_rep += "}";
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

void SerializeComponent(Velocity c, std::string& str_rep)
{
	str_rep += "Velocity{";
	str_rep += "velocity=";
	str_rep += ToString(c.velocity);
	str_rep += "}";
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

void SerializeComponent(Acceleration c, std::string& str_rep)
{
	str_rep += "Acceleration{";
	str_rep += "acceleration=";
	str_rep += ToString(c.acceleration);
	str_rep += "}";
}

void DeserializeComponent(Acceleration& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "acceleration")
		{
			FromString(c.acceleration, statement_parts[1]);
		}
	}
}

void SerializeComponent(ReceivedForces c, std::string& str_rep)
{
	str_rep += "ReceivedForces{";
	str_rep += "mass=";
	str_rep += ToString(c.mass);
	str_rep += ";";
	str_rep += "electric_force=";
	str_rep += ToString(c.electric_force);
	str_rep += ";";
	str_rep += "player_force=";
	str_rep += ToString(c.player_force);
	str_rep += "}";
}

void DeserializeComponent(ReceivedForces& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "mass")
		{
			FromString(c.mass, statement_parts[1]);
		}

		if (statement_parts[0] == "electric_force")
		{
			FromString(c.electric_force, statement_parts[1]);
		}

		if (statement_parts[0] == "player_force")
		{
			FromString(c.player_force, statement_parts[1]);
		}
	}
}

void SerializeComponent(Charge c, std::string& str_rep)
{
	str_rep += "Charge{";
	str_rep += "charge=";
	str_rep += ToString(c.charge);
	str_rep += "}";
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

void SerializeComponent(Player c, std::string& str_rep)
{
	str_rep += "Player{";
	str_rep += "move_force=";
	str_rep += ToString(c.move_force);
	str_rep += ";";
	str_rep += "default_charge=";
	str_rep += ToString(c.default_charge);
	str_rep += "}";
}

void DeserializeComponent(Player& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "move_force")
		{
			FromString(c.move_force, statement_parts[1]);
		}

		if (statement_parts[0] == "default_charge")
		{
			FromString(c.default_charge, statement_parts[1]);
		}
	}
}

void SerializeComponent(Tag c, std::string& str_rep)
{
	str_rep += "Tag{";
	str_rep += "tag=";
	str_rep += ToString(c.tag);
	str_rep += "}";
}

void DeserializeComponent(Tag& c, std::string str_rep)
{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{
		std::vector<std::string> statement_parts = SplitString(variable, "=");

		if (statement_parts[0] == "tag")
		{
			FromString(c.tag, statement_parts[1]);
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
		f << """ << tag << "":";

		if (tag == "BPEntity")
		{
			SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
		}
		
		if (tag == "BPStaticParticle")
		{
			SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Charge>()[entity_id], entity_string);
		}
		
		if (tag == "BPMovingParticle")
		{
			SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Charge>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Velocity>()[entity_id], entity_string);
		}
		
		if (tag == "BPPlayer")
		{
			SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Charge>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Velocity>()[entity_id], entity_string);
		}
		
		if (tag == "BPWall")
		{
			SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
		}
		
		if (tag == "BPGloal")
		{
			SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
		}
		
		if (tag == "BPLaser")
		{
			SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
			SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
			SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
		}
		
		if (tag == "BPElectricField")
		{
			SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
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

		if (tag == "BPEntity")
		{
			GetComponent<ClickedOn>()[entity_id] = {};
			DeserializeComponent(GetComponent<Tag>()[entity_id],
				GetSubstrBetween(line, "Tag{", "}"));
			DeserializeComponent(GetComponent<Position>()[entity_id],
				GetSubstrBetween(line, "Position{", "}"));
		}
		
		if (tag == "BPStaticParticle")
		{
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<ChargeDependentDrawInfo>()[entity_id] = { "content\\particle_100_red+.png", "content\\particle_100_green.png", "content\\particle_100_green-.png" };
			GetComponent<Editable>()[entity_id] = { true, false, false, sf::Vector2f(0, 0), false };
			GetComponent<Radius>()[entity_id] = { 50 };
			DeserializeComponent(GetComponent<Tag>()[entity_id],
				GetSubstrBetween(line, "Tag{", "}"));
			DeserializeComponent(GetComponent<Position>()[entity_id],
				GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id],
				GetSubstrBetween(line, "Charge{", "}"));
		}
		
		if (tag == "BPMovingParticle")
		{
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<ChargeDependentDrawInfo>()[entity_id] = { "content\\particle_100_red+.png", "content\\particle_100_green.png", "content\\particle_100_green-.png" };
			GetComponent<Editable>()[entity_id] = { true, true, false, sf::Vector2f(0, 0), false };
			GetComponent<Radius>()[entity_id] = { 50 };
			GetComponent<Acceleration>()[entity_id] = {};
			GetComponent<ReceivedForces>()[entity_id] = {};
			DeserializeComponent(GetComponent<Tag>()[entity_id],
				GetSubstrBetween(line, "Tag{", "}"));
			DeserializeComponent(GetComponent<Position>()[entity_id],
				GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id],
				GetSubstrBetween(line, "Charge{", "}"));
			DeserializeComponent(GetComponent<Velocity>()[entity_id],
				GetSubstrBetween(line, "Velocity{", "}"));
		}
		
		if (tag == "BPPlayer")
		{
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<ChargeDependentDrawInfo>()[entity_id] = { "content\\particle_100_blue+.png", "content\\particle_100_blue.png", "content\\particle_100_blue-.png" };
			GetComponent<Editable>()[entity_id] = { true, true, false, sf::Vector2f(0, 0), false };
			GetComponent<Radius>()[entity_id] = { 50 };
			GetComponent<Acceleration>()[entity_id] = {};
			GetComponent<ReceivedForces>()[entity_id] = {};
			GetComponent<Player>()[entity_id] = {};
			DeserializeComponent(GetComponent<Tag>()[entity_id],
				GetSubstrBetween(line, "Tag{", "}"));
			DeserializeComponent(GetComponent<Position>()[entity_id],
				GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<Charge>()[entity_id],
				GetSubstrBetween(line, "Charge{", "}"));
			DeserializeComponent(GetComponent<Velocity>()[entity_id],
				GetSubstrBetween(line, "Velocity{", "}"));
		}
		
		if (tag == "BPWall")
		{
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<DrawInfo>()[entity_id] = { "content\\block.png" };
			DeserializeComponent(GetComponent<Tag>()[entity_id],
				GetSubstrBetween(line, "Tag{", "}"));
			DeserializeComponent(GetComponent<Position>()[entity_id],
				GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
				GetSubstrBetween(line, "WidthAndHeight{", "}"));
		}
		
		if (tag == "BPGloal")
		{
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<DrawInfo>()[entity_id] = { "content\\goal.png" };
			DeserializeComponent(GetComponent<Tag>()[entity_id],
				GetSubstrBetween(line, "Tag{", "}"));
			DeserializeComponent(GetComponent<Position>()[entity_id],
				GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
				GetSubstrBetween(line, "WidthAndHeight{", "}"));
		}
		
		if (tag == "BPLaser")
		{
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<OrientationDependentDrawInfo>()[entity_id] = { "content\\laser_horisontal.png", "content\\laser_vertical.png" };
			GetComponent<KillOnIntersection>()[entity_id] = {};
			DeserializeComponent(GetComponent<Tag>()[entity_id],
				GetSubstrBetween(line, "Tag{", "}"));
			DeserializeComponent(GetComponent<Position>()[entity_id],
				GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
				GetSubstrBetween(line, "WidthAndHeight{", "}"));
		}
		
		if (tag == "BPElectricField")
		{
			GetComponent<ClickedOn>()[entity_id] = {};
			GetComponent<DrawInfo>()[entity_id] = { "content\\electric_field.png" };
			DeserializeComponent(GetComponent<Tag>()[entity_id],
				GetSubstrBetween(line, "Tag{", "}"));
			DeserializeComponent(GetComponent<Position>()[entity_id],
				GetSubstrBetween(line, "Position{", "}"));
			DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
				GetSubstrBetween(line, "WidthAndHeight{", "}"));
		}
		
	}
}
