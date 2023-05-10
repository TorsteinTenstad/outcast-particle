
#include <cassert>
#include <string>
#include <vector>
#include <fstream>

#include "level.hpp"
#include "blueprint.hpp"
#include "utils/string_parsing.hpp"
#include "components/_used_in_serialization.hpp"

/*
Warning!
Code is generated.
Changes will be overwritten.
*/


void SerializeComponent(const Tag* c, std::string& str_rep)
{
	str_rep += "Tag{";
	str_rep += "tag=";
	str_rep += ToString(c->tag);
	str_rep += "}";
}

void DeserializeComponent(Tag* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "Tag{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "tag")
        {
            FromString(c->tag, statement_parts[1]);
        }
    }
}
void SerializeComponent(const Position* c, std::string& str_rep)
{
	str_rep += "Position{";
	str_rep += "position=";
	str_rep += ToString(c->position);
	str_rep += "}";
}

void DeserializeComponent(Position* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "Position{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "position")
        {
            FromString(c->position, statement_parts[1]);
        }
    }
}
void SerializeComponent(const Charge* c, std::string& str_rep)
{
	str_rep += "Charge{";
	str_rep += "charge=";
	str_rep += ToString(c->charge);
	str_rep += "}";
}

void DeserializeComponent(Charge* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "Charge{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "charge")
        {
            FromString(c->charge, statement_parts[1]);
        }
    }
}
void SerializeComponent(const Velocity* c, std::string& str_rep)
{
	str_rep += "Velocity{";
	str_rep += "velocity=";
	str_rep += ToString(c->velocity);
	str_rep += "}";
}

void DeserializeComponent(Velocity* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "Velocity{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "velocity")
        {
            FromString(c->velocity, statement_parts[1]);
        }
    }
}
void SerializeComponent(const WidthAndHeight* c, std::string& str_rep)
{
	str_rep += "WidthAndHeight{";
	str_rep += "width_and_height=";
	str_rep += ToString(c->width_and_height);
	str_rep += "}";
}

void DeserializeComponent(WidthAndHeight* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "WidthAndHeight{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "width_and_height")
        {
            FromString(c->width_and_height, statement_parts[1]);
        }
    }
}
void SerializeComponent(const Player* c, std::string& str_rep)
{
	str_rep += "Player{";
	str_rep += "can_switch_charge=";
	str_rep += ToString(c->can_switch_charge);
	str_rep += ";";
	str_rep += "can_go_neutral=";
	str_rep += ToString(c->can_go_neutral);
	str_rep += ";";
	str_rep += "move_force=";
	str_rep += ToString(c->move_force);
	str_rep += "}";
}

void DeserializeComponent(Player* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "Player{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "can_switch_charge")
        {
            FromString(c->can_switch_charge, statement_parts[1]);
        }
        else if (statement_parts[0] == "can_go_neutral")
        {
            FromString(c->can_go_neutral, statement_parts[1]);
        }
        else if (statement_parts[0] == "move_force")
        {
            FromString(c->move_force, statement_parts[1]);
        }
    }
}
void SerializeComponent(const Collision* c, std::string& str_rep)
{
	str_rep += "Collision{";
	str_rep += "bounce_factor=";
	str_rep += ToString(c->bounce_factor);
	str_rep += ";";
	str_rep += "friction=";
	str_rep += ToString(c->friction);
	str_rep += "}";
}

void DeserializeComponent(Collision* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "Collision{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "bounce_factor")
        {
            FromString(c->bounce_factor, statement_parts[1]);
        }
        else if (statement_parts[0] == "friction")
        {
            FromString(c->friction, statement_parts[1]);
        }
    }
}
void SerializeComponent(const ElectricField* c, std::string& str_rep)
{
	str_rep += "ElectricField{";
	str_rep += "field_vector=";
	str_rep += ToString(c->field_vector);
	str_rep += "}";
}

void DeserializeComponent(ElectricField* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "ElectricField{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "field_vector")
        {
            FromString(c->field_vector, statement_parts[1]);
        }
    }
}
void SerializeComponent(const MagneticField* c, std::string& str_rep)
{
	str_rep += "MagneticField{";
	str_rep += "field_strength=";
	str_rep += ToString(c->field_strength);
	str_rep += "}";
}

void DeserializeComponent(MagneticField* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "MagneticField{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "field_strength")
        {
            FromString(c->field_strength, statement_parts[1]);
        }
    }
}
void SerializeComponent(const Text* c, std::string& str_rep)
{
	str_rep += "Text{";
	str_rep += "size=";
	str_rep += ToString(c->size);
	str_rep += ";";
	str_rep += "color=";
	str_rep += ToString(c->color);
	str_rep += ";";
	str_rep += "outline_color=";
	str_rep += ToString(c->outline_color);
	str_rep += ";";
	str_rep += "outline_thickness=";
	str_rep += ToString(c->outline_thickness);
	str_rep += ";";
	str_rep += "font_path=";
	str_rep += ToString(c->font_path);
	str_rep += ";";
	str_rep += "apply_shader=";
	str_rep += ToString(c->apply_shader);
	str_rep += ";";
	str_rep += "content=";
	str_rep += ToString(c->content);
	str_rep += "}";
}

void DeserializeComponent(Text* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "Text{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "size")
        {
            FromString(c->size, statement_parts[1]);
        }
        else if (statement_parts[0] == "color")
        {
            FromString(c->color, statement_parts[1]);
        }
        else if (statement_parts[0] == "outline_color")
        {
            FromString(c->outline_color, statement_parts[1]);
        }
        else if (statement_parts[0] == "outline_thickness")
        {
            FromString(c->outline_thickness, statement_parts[1]);
        }
        else if (statement_parts[0] == "font_path")
        {
            FromString(c->font_path, statement_parts[1]);
        }
        else if (statement_parts[0] == "apply_shader")
        {
            FromString(c->apply_shader, statement_parts[1]);
        }
        else if (statement_parts[0] == "content")
        {
            FromString(c->content, statement_parts[1]);
        }
    }
}
void SerializeComponent(const TextPopupSpawner* c, std::string& str_rep)
{
	str_rep += "TextPopupSpawner{";
	str_rep += "content=";
	str_rep += ToString(c->content);
	str_rep += "}";
}

void DeserializeComponent(TextPopupSpawner* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "TextPopupSpawner{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "content")
        {
            FromString(c->content, statement_parts[1]);
        }
    }
}

void Level::SaveToFile(std::string savefile_path)
{
    std::ofstream f(savefile_path);
    f << "name=" << name << ";grid_size_id=" << ToString(grid_size_id) << ";editable=" << ToString(editable) << "\n";

    std::string entity_string;
    for (auto& [entity_id, tag] : GetEntitiesWith<Tag>())
    {
        if (HasComponent<NotSerialized>(entity_id)){
            continue;
        }
        f << "\"" << tag->tag << "\":";

        if (tag->tag == "BPEditableEntity")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPStaticParticle")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<Charge>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPMovingParticle")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<Charge>(entity_id), entity_string);
            SerializeComponent(GetComponent<Velocity>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPBlackHole")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPPlayer")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<Charge>(entity_id), entity_string);
            SerializeComponent(GetComponent<Velocity>(entity_id), entity_string);
            SerializeComponent(GetComponent<Player>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPLaser")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPCoin")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPWall")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<Collision>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPGoal")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPElectricField")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<ElectricField>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPMagneticField")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<MagneticField>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPText")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
            SerializeComponent(GetComponent<Text>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPTextPopupSpawner")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
            SerializeComponent(GetComponent<TextPopupSpawner>(entity_id), entity_string);
        }
        
        f << entity_string << "\n";
        entity_string.clear();
    }
}

void Level::LoadFromFile(std::string savefile_path)
{
    savefile_path_ = savefile_path;
    Clear();

    std::ifstream f(savefile_path);
    std::string line;

    getline(f, line);
    std::vector<std::string> level_properties = SplitString(line, ";");
    for (auto& property_str : level_properties)
	{
		std::vector<std::string> property_statement_parts = SplitString(property_str, "=");
		if (property_statement_parts[0] == "name")
		{
			FromString(name, property_statement_parts[1]);
		}
		else if (property_statement_parts[0] == "grid_size_id")
		{
			FromString(grid_size_id, property_statement_parts[1]);
		}
		else if (property_statement_parts[0] == "editable")
		{
			FromString(editable, property_statement_parts[1]);
		}
		else
		{
			assert(property_statement_parts[0].length() == 0);
		}
	}

    while (getline(f, line))
    {
        int entity_id = CreateEntityId();
        std::string tag = GetSubstrBetween(line, "\"", "\"");
        AddComponent<Tag>(entity_id)->tag = tag;

        if (tag == "BPEditableEntity")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            DeserializeComponent(AddComponent<Position>(entity_id),line);
        }
        
        if (tag == "BPStaticParticle")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\particle.frag", {}, {}, {} });
            AddComponent<DrawPriority>(entity_id, { 10 });
            AddComponent<ChargeDependentDrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 108 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<Charge>(entity_id),line);
        }
        
        if (tag == "BPMovingParticle")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\particle.frag", {}, {}, {} });
            AddComponent<DrawPriority>(entity_id, { 11 });
            AddComponent<ChargeDependentDrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 108 });
            AddComponent<Acceleration>(entity_id, {});
            AddComponent<ReceivedForces>(entity_id, {});
            AddComponent<Mass>(entity_id, {});
            AddComponent<Intersection>(entity_id, {});
            AddComponent<Collision>(entity_id, {});
            AddComponent<Trail>(entity_id, {});
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<Charge>(entity_id),line);
            DeserializeComponent(AddComponent<Velocity>(entity_id),line);
        }
        
        if (tag == "BPBlackHole")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\black_hole.frag", {}, {}, {} });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPPlayer")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 108 });
            AddComponent<Acceleration>(entity_id, {});
            AddComponent<ReceivedForces>(entity_id, {});
            AddComponent<Mass>(entity_id, {});
            AddComponent<Intersection>(entity_id, {});
            AddComponent<Collision>(entity_id, {});
            AddComponent<Children>(entity_id, {});
            AddComponent<Trail>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 12 });
            AddComponent<Shader>(entity_id, { "shaders\\player.vert", "shaders\\particle.frag", {}, {}, {} });
            AddComponent<SoundInfo>(entity_id, { { { DEFAULT, "content\\sounds\\wav.wav" }, { TO_NEUTRAL, "content\\sounds\\to_neutral.wav" }, { FROM_NEUTRAL, "content\\sounds\\from_neutral.wav" } } });
            AddComponent<Face>(entity_id, {});
            AddComponent<ForceVisualization>(entity_id, {});
            AddComponent<PlayerBehaviors>(entity_id, {});
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<Charge>(entity_id),line);
            DeserializeComponent(AddComponent<Velocity>(entity_id),line);
            DeserializeComponent(AddComponent<Player>(entity_id),line);
        }
        
        if (tag == "BPLaser")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, { 60 });
            AddComponent<Laser>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 3 });
            AddComponent<OrientationDependentDrawInfo>(entity_id, {});
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPCoin")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 5 });
            AddComponent<Coin>(entity_id, {});
            AddComponent<SegmentedGlowEffect>(entity_id, {});
            AddComponent<Children>(entity_id, {});
            AddComponent<Radius>(entity_id, { 60 });
            AddComponent<SoundInfo>(entity_id, { { { DEFAULT, "content\\sounds\\coin.wav" } } });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
        }
        
        if (tag == "BPWall")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Wall>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 4 });
            AddComponent<SoundInfo>(entity_id, { { { DEFAULT, "content\\sounds\\wall_3.wav" } } });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<Collision>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPGoal")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\wormhole.frag", {}, {}, {} });
            AddComponent<DrawPriority>(entity_id, { 2 });
            AddComponent<Goal>(entity_id, {});
            AddComponent<Wormhole>(entity_id, {});
            AddComponent<Mass>(entity_id, {});
            AddComponent<SoundInfo>(entity_id, { { { DEFAULT, "content\\sounds\\happy_transition.wav" } } });
            AddComponent<Radius>(entity_id, { 240 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
        }
        
        if (tag == "BPElectricField")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\electric_field.frag", {}, {}, {} });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<ElectricField>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPMagneticField")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\magnetic_field.frag", {}, {}, {} });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<MagneticField>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPText")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\transparent.png", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 100 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
            DeserializeComponent(AddComponent<Text>(entity_id),line);
        }
        
        if (tag == "BPTextPopupSpawner")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\transparent.png", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 2 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
            DeserializeComponent(AddComponent<TextPopupSpawner>(entity_id),line);
        }
        
    }
}

int Level::AddBlueprint(std::string blueprint_tag){
    return AddBlueprint(ToBlueprintEnum(blueprint_tag));
}

int Level::AddBlueprint(Blueprint blueprint)
{
    int entity_id = CreateEntityId();
    switch (blueprint){
        case BPEditableEntity:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPEditableEntity"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            break;
        case BPStaticParticle:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\particle.frag", {}, {}, {} });
            AddComponent<DrawPriority>(entity_id, { 10 });
            AddComponent<ChargeDependentDrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 108 });
            AddComponent<Tag>(entity_id, {"BPStaticParticle"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Charge>(entity_id, { 10000 });
            break;
        case BPMovingParticle:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\particle.frag", {}, {}, {} });
            AddComponent<DrawPriority>(entity_id, { 11 });
            AddComponent<ChargeDependentDrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 108 });
            AddComponent<Acceleration>(entity_id, {});
            AddComponent<ReceivedForces>(entity_id, {});
            AddComponent<Mass>(entity_id, {});
            AddComponent<Intersection>(entity_id, {});
            AddComponent<Collision>(entity_id, {});
            AddComponent<Trail>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPMovingParticle"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Charge>(entity_id, { 10000 });
            AddComponent<Velocity>(entity_id, { sf::Vector2f(0, 0) });
            break;
        case BPBlackHole:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\black_hole.frag", {}, {}, {} });
            AddComponent<Tag>(entity_id, {"BPBlackHole"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(240, 240) });
            break;
        case BPPlayer:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 108 });
            AddComponent<Acceleration>(entity_id, {});
            AddComponent<ReceivedForces>(entity_id, {});
            AddComponent<Mass>(entity_id, {});
            AddComponent<Intersection>(entity_id, {});
            AddComponent<Collision>(entity_id, {});
            AddComponent<Children>(entity_id, {});
            AddComponent<Trail>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 12 });
            AddComponent<Shader>(entity_id, { "shaders\\player.vert", "shaders\\particle.frag", {}, {}, {} });
            AddComponent<SoundInfo>(entity_id, { { { DEFAULT, "content\\sounds\\wav.wav" }, { TO_NEUTRAL, "content\\sounds\\to_neutral.wav" }, { FROM_NEUTRAL, "content\\sounds\\from_neutral.wav" } } });
            AddComponent<Face>(entity_id, {});
            AddComponent<ForceVisualization>(entity_id, {});
            AddComponent<PlayerBehaviors>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPPlayer"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Charge>(entity_id, { 10000 });
            AddComponent<Velocity>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Player>(entity_id, {});
            break;
        case BPLaser:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, { 60 });
            AddComponent<Laser>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 3 });
            AddComponent<OrientationDependentDrawInfo>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPLaser"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 60) });
            break;
        case BPCoin:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 5 });
            AddComponent<Coin>(entity_id, {});
            AddComponent<SegmentedGlowEffect>(entity_id, {});
            AddComponent<Children>(entity_id, {});
            AddComponent<Radius>(entity_id, { 60 });
            AddComponent<SoundInfo>(entity_id, { { { DEFAULT, "content\\sounds\\coin.wav" } } });
            AddComponent<Tag>(entity_id, {"BPCoin"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            break;
        case BPWall:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Wall>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 4 });
            AddComponent<SoundInfo>(entity_id, { { { DEFAULT, "content\\sounds\\wall_3.wav" } } });
            AddComponent<Tag>(entity_id, {"BPWall"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Collision>(entity_id, { 0.2, 75 });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
            break;
        case BPGoal:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\wormhole.frag", {}, {}, {} });
            AddComponent<DrawPriority>(entity_id, { 2 });
            AddComponent<Goal>(entity_id, {});
            AddComponent<Wormhole>(entity_id, {});
            AddComponent<Mass>(entity_id, {});
            AddComponent<SoundInfo>(entity_id, { { { DEFAULT, "content\\sounds\\happy_transition.wav" } } });
            AddComponent<Radius>(entity_id, { 240 });
            AddComponent<Tag>(entity_id, {"BPGoal"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            break;
        case BPElectricField:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\electric_field.frag", {}, {}, {} });
            AddComponent<Tag>(entity_id, {"BPElectricField"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<ElectricField>(entity_id, { sf::Vector2f(0, 0.25) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(240, 240) });
            break;
        case BPMagneticField:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\magnetic_field.frag", {}, {}, {} });
            AddComponent<Tag>(entity_id, {"BPMagneticField"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<MagneticField>(entity_id, { 0.1 });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(240, 240) });
            break;
        case BPText:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\transparent.png", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 100 });
            AddComponent<Tag>(entity_id, {"BPText"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(240, 240) });
            AddComponent<Text>(entity_id, {});
            break;
        case BPTextPopupSpawner:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\transparent.png", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 2 });
            AddComponent<Tag>(entity_id, {"BPTextPopupSpawner"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
            AddComponent<TextPopupSpawner>(entity_id, { "ipsum lorem" });
            break;
        default:
            assert(false);
    }
    return entity_id;
}