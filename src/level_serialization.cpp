
#include <string>
#include <vector>
#include <fstream>

#include "level.hpp"
#include "string_parsing_utils.hpp"

/*
Warning!
Code is generated.
Changes will be overwritten.
*/


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

void SerializeComponent(Player c, std::string& str_rep)
{
	str_rep += "Player{";
	str_rep += "can_switch_charge=";
	str_rep += ToString(c.can_switch_charge);
	str_rep += ";";
	str_rep += "can_go_neutral=";
	str_rep += ToString(c.can_go_neutral);
	str_rep += ";";
	str_rep += "move_force=";
	str_rep += ToString(c.move_force);
	str_rep += "}";
}

void DeserializeComponent(Player& c, std::string str_rep)
{
    std::vector<std::string> variables = SplitString(str_rep, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "can_switch_charge")
        {
            FromString(c.can_switch_charge, statement_parts[1]);
        }

        if (statement_parts[0] == "can_go_neutral")
        {
            FromString(c.can_go_neutral, statement_parts[1]);
        }

        if (statement_parts[0] == "move_force")
        {
            FromString(c.move_force, statement_parts[1]);
        }
	}
}

void SerializeComponent(ElectricField c, std::string& str_rep)
{
	str_rep += "ElectricField{";
	str_rep += "field_vector=";
	str_rep += ToString(c.field_vector);
	str_rep += "}";
}

void DeserializeComponent(ElectricField& c, std::string str_rep)
{
    std::vector<std::string> variables = SplitString(str_rep, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "field_vector")
        {
            FromString(c.field_vector, statement_parts[1]);
        }
	}
}

void SerializeComponent(MagneticField c, std::string& str_rep)
{
	str_rep += "MagneticField{";
	str_rep += "field_strength=";
	str_rep += ToString(c.field_strength);
	str_rep += "}";
}

void DeserializeComponent(MagneticField& c, std::string str_rep)
{
    std::vector<std::string> variables = SplitString(str_rep, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "field_strength")
        {
            FromString(c.field_strength, statement_parts[1]);
        }
	}
}


void Level::SaveToFile(std::string savefile_path)
{
    std::ofstream f(savefile_path);
    f << "name=" << name << ";size=" << ToString(size) << ";editable=" << ToString(editable) << "\n";

    std::map<int, Tag>& tags = GetComponent<Tag>();
    std::string entity_string;
    for (auto& [entity_id, tag_component] : tags)
    {
        std::string tag = tag_component.tag;
        f << "\"" << tag << "\":";

        if (tag == "BPEntity")
        {
            SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
            SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
        }
        
        if (tag == "BPTileBasedEntity")
        {
            SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
            SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
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
            SerializeComponent(GetComponent<Player>()[entity_id], entity_string);
        }
        
        if (tag == "BPLaser")
        {
            SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
            SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
        }
        
        if (tag == "BPWall")
        {
            SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
            SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
        }
        
        if (tag == "BPGoal")
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
            SerializeComponent(GetComponent<ElectricField>()[entity_id], entity_string);
        }
        
        if (tag == "BPMagneticField")
        {
            SerializeComponent(GetComponent<Tag>()[entity_id], entity_string);
            SerializeComponent(GetComponent<Position>()[entity_id], entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>()[entity_id], entity_string);
            SerializeComponent(GetComponent<MagneticField>()[entity_id], entity_string);
        }
        
        f << entity_string << "\n";
        entity_string.clear();
    }
}

void Level::LoadFromFile(std::string savefile_path)
{
    savefile_path_ = savefile_path;
    for (auto& [_, component_map_variant] : components_)
    {
        std::visit([](auto& component_map) { component_map.clear(); }, component_map_variant);
    }

    std::ifstream f(savefile_path);
    std::string line;

    getline(f, line);
    std::vector<std::string> level_properties = SplitString(line, ";");
    for (auto& property_str : level_properties)
    {
        property_str = SplitString(property_str, "=")[1];
    }
    FromString(name, level_properties[0]);
    FromString(size, level_properties[1]);
    FromString(editable, level_properties[2]);

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
        
        if (tag == "BPTileBasedEntity")
        {
            GetComponent<ClickedOn>()[entity_id] = {};
            GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
            DeserializeComponent(GetComponent<Tag>()[entity_id],
                GetSubstrBetween(line, "Tag{", "}"));
            DeserializeComponent(GetComponent<Position>()[entity_id],
                GetSubstrBetween(line, "Position{", "}"));
            DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
                GetSubstrBetween(line, "WidthAndHeight{", "}"));
        }
        
        if (tag == "BPStaticParticle")
        {
            GetComponent<ClickedOn>()[entity_id] = {};
            GetComponent<DrawInfo>()[entity_id] = { "_", false, 0 };
            GetComponent<DrawPriority>()[entity_id] = { 6 };
            GetComponent<ChargeDependentDrawInfo>()[entity_id] = {};
            GetComponent<Editable>()[entity_id] = { true, true, false, 0, sf::Vector2f(0, 0), false };
            GetComponent<Radius>()[entity_id] = { 120 };
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
            GetComponent<DrawInfo>()[entity_id] = { "_", false, 0 };
            GetComponent<DrawPriority>()[entity_id] = { 7 };
            GetComponent<ChargeDependentDrawInfo>()[entity_id] = {};
            GetComponent<Editable>()[entity_id] = { true, true, true, 0, sf::Vector2f(0, 0), false };
            GetComponent<Radius>()[entity_id] = { 120 };
            GetComponent<Acceleration>()[entity_id] = {};
            GetComponent<ReceivedForces>()[entity_id] = {};
            GetComponent<Intersection>()[entity_id] = {};
            GetComponent<Collision>()[entity_id] = {};
            GetComponent<Trail>()[entity_id] = {};
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
            GetComponent<DrawInfo>()[entity_id] = { "_", false, 0 };
            GetComponent<DrawPriority>()[entity_id] = { 8 };
            GetComponent<ChargeDependentDrawInfo>()[entity_id] = {};
            GetComponent<Editable>()[entity_id] = { true, true, true, 0, sf::Vector2f(0, 0), false };
            GetComponent<Radius>()[entity_id] = { 120 };
            GetComponent<Acceleration>()[entity_id] = {};
            GetComponent<ReceivedForces>()[entity_id] = {};
            GetComponent<Intersection>()[entity_id] = {};
            GetComponent<Collision>()[entity_id] = {};
            GetComponent<Trail>()[entity_id] = {};
            GetComponent<Shader>()[entity_id] = { "shaders\\player.vert", "shaders\\player.frag", {}, { { "_time", 0 } }, {} };
            GetComponent<SoundInfo>()[entity_id] = { "content\\sounds\\wav.wav" };
            GetComponent<VelocityDependentDrawLayer>()[entity_id] = {};
            GetComponent<ForceVisualization>()[entity_id] = {};
            GetComponent<PlayerBehaviors>()[entity_id] = {};
            DeserializeComponent(GetComponent<Tag>()[entity_id],
                GetSubstrBetween(line, "Tag{", "}"));
            DeserializeComponent(GetComponent<Position>()[entity_id],
                GetSubstrBetween(line, "Position{", "}"));
            DeserializeComponent(GetComponent<Charge>()[entity_id],
                GetSubstrBetween(line, "Charge{", "}"));
            DeserializeComponent(GetComponent<Velocity>()[entity_id],
                GetSubstrBetween(line, "Velocity{", "}"));
            DeserializeComponent(GetComponent<Player>()[entity_id],
                GetSubstrBetween(line, "Player{", "}"));
        }
        
        if (tag == "BPLaser")
        {
            GetComponent<ClickedOn>()[entity_id] = {};
            GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\laser_horisontal.png", false, 0 };
            GetComponent<DrawPriority>()[entity_id] = { 3 };
            GetComponent<OrientationDependentDrawInfo>()[entity_id] = {};
            GetComponent<Editable>()[entity_id] = { true, false, false, 60, sf::Vector2f(0, 0), false };
            GetComponent<KillOnIntersection>()[entity_id] = {};
            GetComponent<Shader>()[entity_id] = { "shaders\\test.vert", "", {}, { { "_time", 0 } }, {} };
            GetComponent<SoundInfo>()[entity_id] = { "content\\sounds\\laser.wav" };
            DeserializeComponent(GetComponent<Tag>()[entity_id],
                GetSubstrBetween(line, "Tag{", "}"));
            DeserializeComponent(GetComponent<Position>()[entity_id],
                GetSubstrBetween(line, "Position{", "}"));
            DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
                GetSubstrBetween(line, "WidthAndHeight{", "}"));
        }
        
        if (tag == "BPWall")
        {
            GetComponent<ClickedOn>()[entity_id] = {};
            GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
            GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\block.png", false, 0 };
            GetComponent<DrawPriority>()[entity_id] = { 4 };
            GetComponent<SoundInfo>()[entity_id] = { "content\\sounds\\thud.wav" };
            GetComponent<Collision>()[entity_id] = { 0.2, 75 };
            DeserializeComponent(GetComponent<Tag>()[entity_id],
                GetSubstrBetween(line, "Tag{", "}"));
            DeserializeComponent(GetComponent<Position>()[entity_id],
                GetSubstrBetween(line, "Position{", "}"));
            DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
                GetSubstrBetween(line, "WidthAndHeight{", "}"));
        }
        
        if (tag == "BPGoal")
        {
            GetComponent<ClickedOn>()[entity_id] = {};
            GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
            GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\goal.png", false, 0 };
            GetComponent<DrawPriority>()[entity_id] = { 2 };
            GetComponent<Goal>()[entity_id] = {};
            GetComponent<KillOnIntersection>()[entity_id] = {};
            GetComponent<SoundInfo>()[entity_id] = { "content\\sounds\\happy_transition.wav" };
            GetComponent<SegmentedGlowEffect>()[entity_id] = {};
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
            GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
            GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\electric_field.png", false, 0 };
            GetComponent<DrawPriority>()[entity_id] = { 1 };
            DeserializeComponent(GetComponent<Tag>()[entity_id],
                GetSubstrBetween(line, "Tag{", "}"));
            DeserializeComponent(GetComponent<Position>()[entity_id],
                GetSubstrBetween(line, "Position{", "}"));
            DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
                GetSubstrBetween(line, "WidthAndHeight{", "}"));
            DeserializeComponent(GetComponent<ElectricField>()[entity_id],
                GetSubstrBetween(line, "ElectricField{", "}"));
        }
        
        if (tag == "BPMagneticField")
        {
            GetComponent<ClickedOn>()[entity_id] = {};
            GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
            GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\magnetic_field.png", false, 0 };
            GetComponent<DrawPriority>()[entity_id] = { 1 };
            DeserializeComponent(GetComponent<Tag>()[entity_id],
                GetSubstrBetween(line, "Tag{", "}"));
            DeserializeComponent(GetComponent<Position>()[entity_id],
                GetSubstrBetween(line, "Position{", "}"));
            DeserializeComponent(GetComponent<WidthAndHeight>()[entity_id],
                GetSubstrBetween(line, "WidthAndHeight{", "}"));
            DeserializeComponent(GetComponent<MagneticField>()[entity_id],
                GetSubstrBetween(line, "MagneticField{", "}"));
        }
        
    }
}

int Level::AddBlueprint(std::string tag)
{
    int entity_id = CreateEntityId();
    if (tag == "BPEntity")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<Tag>()[entity_id] = {"BPEntity"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        return entity_id;
    }
    if (tag == "BPTileBasedEntity")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
        GetComponent<Tag>()[entity_id] = {"BPTileBasedEntity"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<WidthAndHeight>()[entity_id] = { sf::Vector2f(120, 120) };
        return entity_id;
    }
    if (tag == "BPStaticParticle")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<DrawInfo>()[entity_id] = { "_", false, 0 };
        GetComponent<DrawPriority>()[entity_id] = { 6 };
        GetComponent<ChargeDependentDrawInfo>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, true, false, 0, sf::Vector2f(0, 0), false };
        GetComponent<Radius>()[entity_id] = { 120 };
        GetComponent<Tag>()[entity_id] = {"BPStaticParticle"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<Charge>()[entity_id] = { 10000 };
        return entity_id;
    }
    if (tag == "BPMovingParticle")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<DrawInfo>()[entity_id] = { "_", false, 0 };
        GetComponent<DrawPriority>()[entity_id] = { 7 };
        GetComponent<ChargeDependentDrawInfo>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, true, true, 0, sf::Vector2f(0, 0), false };
        GetComponent<Radius>()[entity_id] = { 120 };
        GetComponent<Acceleration>()[entity_id] = {};
        GetComponent<ReceivedForces>()[entity_id] = {};
        GetComponent<Intersection>()[entity_id] = {};
        GetComponent<Collision>()[entity_id] = {};
        GetComponent<Trail>()[entity_id] = {};
        GetComponent<Tag>()[entity_id] = {"BPMovingParticle"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<Charge>()[entity_id] = { 10000 };
        GetComponent<Velocity>()[entity_id] = { sf::Vector2f(0, 0) };
        return entity_id;
    }
    if (tag == "BPPlayer")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<DrawInfo>()[entity_id] = { "_", false, 0 };
        GetComponent<DrawPriority>()[entity_id] = { 8 };
        GetComponent<ChargeDependentDrawInfo>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, true, true, 0, sf::Vector2f(0, 0), false };
        GetComponent<Radius>()[entity_id] = { 120 };
        GetComponent<Acceleration>()[entity_id] = {};
        GetComponent<ReceivedForces>()[entity_id] = {};
        GetComponent<Intersection>()[entity_id] = {};
        GetComponent<Collision>()[entity_id] = {};
        GetComponent<Trail>()[entity_id] = {};
        GetComponent<Shader>()[entity_id] = { "shaders\\player.vert", "shaders\\player.frag", {}, { { "_time", 0 } }, {} };
        GetComponent<SoundInfo>()[entity_id] = { "content\\sounds\\wav.wav" };
        GetComponent<VelocityDependentDrawLayer>()[entity_id] = {};
        GetComponent<ForceVisualization>()[entity_id] = {};
        GetComponent<PlayerBehaviors>()[entity_id] = {};
        GetComponent<Tag>()[entity_id] = {"BPPlayer"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<Charge>()[entity_id] = { 10000 };
        GetComponent<Velocity>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<Player>()[entity_id] = { true, true, 1000 };
        return entity_id;
    }
    if (tag == "BPLaser")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\laser_horisontal.png", false, 0 };
        GetComponent<DrawPriority>()[entity_id] = { 3 };
        GetComponent<OrientationDependentDrawInfo>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, false, false, 60, sf::Vector2f(0, 0), false };
        GetComponent<KillOnIntersection>()[entity_id] = {};
        GetComponent<Shader>()[entity_id] = { "shaders\\test.vert", "", {}, { { "_time", 0 } }, {} };
        GetComponent<SoundInfo>()[entity_id] = { "content\\sounds\\laser.wav" };
        GetComponent<Tag>()[entity_id] = {"BPLaser"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<WidthAndHeight>()[entity_id] = { sf::Vector2f(120, 60) };
        return entity_id;
    }
    if (tag == "BPWall")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
        GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\block.png", false, 0 };
        GetComponent<DrawPriority>()[entity_id] = { 4 };
        GetComponent<SoundInfo>()[entity_id] = { "content\\sounds\\thud.wav" };
        GetComponent<Collision>()[entity_id] = { 0.2, 75 };
        GetComponent<Tag>()[entity_id] = {"BPWall"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<WidthAndHeight>()[entity_id] = { sf::Vector2f(120, 120) };
        return entity_id;
    }
    if (tag == "BPGoal")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
        GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\goal.png", false, 0 };
        GetComponent<DrawPriority>()[entity_id] = { 2 };
        GetComponent<Goal>()[entity_id] = {};
        GetComponent<KillOnIntersection>()[entity_id] = {};
        GetComponent<SoundInfo>()[entity_id] = { "content\\sounds\\happy_transition.wav" };
        GetComponent<SegmentedGlowEffect>()[entity_id] = {};
        GetComponent<Tag>()[entity_id] = {"BPGoal"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<WidthAndHeight>()[entity_id] = { sf::Vector2f(240, 240) };
        return entity_id;
    }
    if (tag == "BPElectricField")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
        GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\electric_field.png", false, 0 };
        GetComponent<DrawPriority>()[entity_id] = { 1 };
        GetComponent<Tag>()[entity_id] = {"BPElectricField"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<WidthAndHeight>()[entity_id] = { sf::Vector2f(240, 240) };
        GetComponent<ElectricField>()[entity_id] = { sf::Vector2f(0, 0.25) };
        return entity_id;
    }
    if (tag == "BPMagneticField")
    {
        GetComponent<ClickedOn>()[entity_id] = {};
        GetComponent<Editable>()[entity_id] = { true, false, false, 120, sf::Vector2f(0, 0), false };
        GetComponent<DrawInfo>()[entity_id] = { "content\\textures\\magnetic_field.png", false, 0 };
        GetComponent<DrawPriority>()[entity_id] = { 1 };
        GetComponent<Tag>()[entity_id] = {"BPMagneticField"};
        GetComponent<Position>()[entity_id] = { sf::Vector2f(0, 0) };
        GetComponent<WidthAndHeight>()[entity_id] = { sf::Vector2f(240, 240) };
        GetComponent<MagneticField>()[entity_id] = { 0.1 };
        return entity_id;
    }
    return entity_id;
}
#pragma GCC diagnostic pop
