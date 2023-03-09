
#include <cassert>
#include <string>
#include <vector>
#include <fstream>

#include "level.hpp"
#include "blueprint.hpp"
#include "string_parsing_utils.hpp"

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
        else {{
            assert(false);
        }}
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
        else {{
            assert(false);
        }}
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
        else {{
            assert(false);
        }}
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
        else {{
            assert(false);
        }}
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
        else {{
            assert(false);
        }}
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
        else {{
            assert(false);
        }}
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
        else {{
            assert(false);
        }}
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
        else {{
            assert(false);
        }}
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
        else {{
            assert(false);
        }}
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

        if (tag->tag == "BPTexturedRectangle")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPButton")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPMenuNavigationButton")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPMenuNavigator")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
        }
        
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
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPBounceWall")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPNoBounceWall")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPGoal")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
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
        
        if (tag->tag == "BPTextPopupSpawner")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
            SerializeComponent(GetComponent<Position>(entity_id), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity_id), entity_string);
            SerializeComponent(GetComponent<TextPopupSpawner>(entity_id), entity_string);
        }
        
        if (tag->tag == "BPText")
        {
            SerializeComponent(GetComponent<Tag>(entity_id), entity_string);
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
    FromString(grid_size_id, level_properties[1]);
    FromString(editable, level_properties[2]);

    while (getline(f, line))
    {
        int entity_id = CreateEntityId();
        std::string tag = GetSubstrBetween(line, "\"", "\"");
        AddComponent<Tag>(entity_id)->tag = tag;

        if (tag == "BPTexturedRectangle")
        {
            AddComponent<DrawPriority>(entity_id, { 0 });
            AddComponent<DrawInfo>(entity_id, { "_", false, 0 });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
            AddComponent<Position>(entity_id, {});
        }
        
        if (tag == "BPButton")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 100 });
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\white.png", false, 0 });
            AddComponent<FillColor>(entity_id, {});
            AddComponent<MouseInteractionDependentFillColor>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\round_corners.frag", {}, {}, {} });
            AddComponent<Text>(entity_id, {});
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(10, 2) * 120.f });
            AddComponent<Position>(entity_id, {});
        }
        
        if (tag == "BPMenuNavigationButton")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 100 });
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\white.png", false, 0 });
            AddComponent<FillColor>(entity_id, {});
            AddComponent<MouseInteractionDependentFillColor>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\round_corners.frag", {}, {}, {} });
            AddComponent<Text>(entity_id, {});
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(10, 2) * 120.f });
            AddComponent<Position>(entity_id, {});
            AddComponent<OnReleasedThisFrame>(entity_id, {});
            AddComponent<MenuNavigatable>(entity_id, {});
        }
        
        if (tag == "BPMenuNavigator")
        {
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\menu_navigator.png", true, 0 });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 180) });
            AddComponent<FillColor>(entity_id, { sf::Color(120, 120, 120) });
            AddComponent<DrawPriority>(entity_id, { 101 });
            AddComponent<MenuNavigator>(entity_id, {});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
        }
        
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
            AddComponent<DrawPriority>(entity_id, { 6 });
            AddComponent<ChargeDependentDrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 120 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<Charge>(entity_id),line);
        }
        
        if (tag == "BPMovingParticle")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\particle.frag", {}, {}, {} });
            AddComponent<DrawPriority>(entity_id, { 7 });
            AddComponent<ChargeDependentDrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 120 });
            AddComponent<Acceleration>(entity_id, {});
            AddComponent<ReceivedForces>(entity_id, {});
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
            AddComponent<Radius>(entity_id, { 120 });
            AddComponent<Acceleration>(entity_id, {});
            AddComponent<ReceivedForces>(entity_id, {});
            AddComponent<Intersection>(entity_id, {});
            AddComponent<Collision>(entity_id, {});
            AddComponent<Children>(entity_id, {});
            AddComponent<Trail>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 8 });
            AddComponent<Shader>(entity_id, { "shaders\\player.vert", "shaders\\player.frag", {}, {}, {} });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\wav.wav" });
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
            AddComponent<KillOnIntersection>(entity_id, {});
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\laser.wav" });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPCoin")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 3 });
            AddComponent<Coin>(entity_id, {});
            AddComponent<SegmentedGlowEffect>(entity_id, {});
            AddComponent<Children>(entity_id, {});
            AddComponent<Radius>(entity_id, { 60 });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\coin.wav" });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
        }
        
        if (tag == "BPWall")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Wall>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 4 });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\thud.wav" });
            AddComponent<Collision>(entity_id, { 0.2, 75 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPBounceWall")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Wall>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 4 });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\thud.wav" });
            AddComponent<Collision>(entity_id, { 1, 75 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPNoBounceWall")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Wall>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 4 });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\thud.wav" });
            AddComponent<Collision>(entity_id, { 0.05, 75 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPGoal")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\goal.png", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 2 });
            AddComponent<Goal>(entity_id, {});
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\happy_transition.wav", false, 1 });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
        }
        
        if (tag == "BPElectricField")
        {
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "", false, 0 });
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
            AddComponent<DrawInfo>(entity_id, { "", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\magnetic_field.frag", {}, {}, {} });
            DeserializeComponent(AddComponent<Position>(entity_id),line);
            DeserializeComponent(AddComponent<MagneticField>(entity_id),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity_id),line);
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
        
        if (tag == "BPText")
        {
            AddComponent<Text>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 101 });
            AddComponent<Shader>(entity_id, { "", "shaders\\scroll.frag", {}, {}, {} });
            AddComponent<Position>(entity_id, {});
        }
        
    }
}

int Level::AddBlueprint(Blueprint blueprint)
{
    int entity_id = CreateEntityId();
    switch (blueprint){
        case BPTexturedRectangle:
            AddComponent<DrawPriority>(entity_id, { 0 });
            AddComponent<DrawInfo>(entity_id, { "_", false, 0 });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
            AddComponent<Position>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPTexturedRectangle"});
            break;
        case BPButton:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 100 });
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\white.png", false, 0 });
            AddComponent<FillColor>(entity_id, {});
            AddComponent<MouseInteractionDependentFillColor>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\round_corners.frag", {}, {}, {} });
            AddComponent<Text>(entity_id, {});
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(10, 2) * 120.f });
            AddComponent<Position>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPButton"});
            break;
        case BPMenuNavigationButton:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 100 });
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\white.png", false, 0 });
            AddComponent<FillColor>(entity_id, {});
            AddComponent<MouseInteractionDependentFillColor>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\round_corners.frag", {}, {}, {} });
            AddComponent<Text>(entity_id, {});
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(10, 2) * 120.f });
            AddComponent<Position>(entity_id, {});
            AddComponent<OnReleasedThisFrame>(entity_id, {});
            AddComponent<MenuNavigatable>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPMenuNavigationButton"});
            break;
        case BPMenuNavigator:
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\menu_navigator.png", true, 0 });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 180) });
            AddComponent<FillColor>(entity_id, { sf::Color(120, 120, 120) });
            AddComponent<DrawPriority>(entity_id, { 101 });
            AddComponent<MenuNavigator>(entity_id, {});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Tag>(entity_id, {"BPMenuNavigator"});
            break;
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
            AddComponent<DrawPriority>(entity_id, { 6 });
            AddComponent<ChargeDependentDrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 120 });
            AddComponent<Tag>(entity_id, {"BPStaticParticle"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Charge>(entity_id, { 10000 });
            break;
        case BPMovingParticle:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, {});
            AddComponent<Shader>(entity_id, { "", "shaders\\particle.frag", {}, {}, {} });
            AddComponent<DrawPriority>(entity_id, { 7 });
            AddComponent<ChargeDependentDrawInfo>(entity_id, {});
            AddComponent<Radius>(entity_id, { 120 });
            AddComponent<Acceleration>(entity_id, {});
            AddComponent<ReceivedForces>(entity_id, {});
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
            AddComponent<Radius>(entity_id, { 120 });
            AddComponent<Acceleration>(entity_id, {});
            AddComponent<ReceivedForces>(entity_id, {});
            AddComponent<Intersection>(entity_id, {});
            AddComponent<Collision>(entity_id, {});
            AddComponent<Children>(entity_id, {});
            AddComponent<Trail>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 8 });
            AddComponent<Shader>(entity_id, { "shaders\\player.vert", "shaders\\player.frag", {}, {}, {} });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\wav.wav" });
            AddComponent<Face>(entity_id, {});
            AddComponent<ForceVisualization>(entity_id, {});
            AddComponent<PlayerBehaviors>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPPlayer"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Charge>(entity_id, { 10000 });
            AddComponent<Velocity>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<Player>(entity_id, { true, true, 1000 });
            break;
        case BPLaser:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, { 60 });
            AddComponent<Laser>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 3 });
            AddComponent<OrientationDependentDrawInfo>(entity_id, {});
            AddComponent<KillOnIntersection>(entity_id, {});
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\laser.wav" });
            AddComponent<Tag>(entity_id, {"BPLaser"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 60) });
            break;
        case BPCoin:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 3 });
            AddComponent<Coin>(entity_id, {});
            AddComponent<SegmentedGlowEffect>(entity_id, {});
            AddComponent<Children>(entity_id, {});
            AddComponent<Radius>(entity_id, { 60 });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\coin.wav" });
            AddComponent<Tag>(entity_id, {"BPCoin"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            break;
        case BPWall:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Wall>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 4 });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\thud.wav" });
            AddComponent<Collision>(entity_id, { 0.2, 75 });
            AddComponent<Tag>(entity_id, {"BPWall"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
            break;
        case BPBounceWall:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Wall>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 4 });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\thud.wav" });
            AddComponent<Collision>(entity_id, { 1, 75 });
            AddComponent<Tag>(entity_id, {"BPBounceWall"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
            break;
        case BPNoBounceWall:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<Wall>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 4 });
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\thud.wav" });
            AddComponent<Collision>(entity_id, { 0.05, 75 });
            AddComponent<Tag>(entity_id, {"BPNoBounceWall"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
            break;
        case BPGoal:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "content\\textures\\goal.png", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 2 });
            AddComponent<Goal>(entity_id, {});
            AddComponent<SoundInfo>(entity_id, { "content\\sounds\\happy_transition.wav", false, 1 });
            AddComponent<Tag>(entity_id, {"BPGoal"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(240, 240) });
            break;
        case BPElectricField:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\electric_field.frag", {}, {}, {} });
            AddComponent<Tag>(entity_id, {"BPElectricField"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<ElectricField>(entity_id, { sf::Vector2f(0, 0.25) });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
            break;
        case BPMagneticField:
            AddComponent<ReceivesButtonEvents>(entity_id, {});
            AddComponent<Editable>(entity_id, {});
            AddComponent<DrawInfo>(entity_id, { "", false, 0 });
            AddComponent<DrawPriority>(entity_id, { 1 });
            AddComponent<Shader>(entity_id, { "", "shaders\\magnetic_field.frag", {}, {}, {} });
            AddComponent<Tag>(entity_id, {"BPMagneticField"});
            AddComponent<Position>(entity_id, { sf::Vector2f(0, 0) });
            AddComponent<MagneticField>(entity_id, { 0.1 });
            AddComponent<WidthAndHeight>(entity_id, { sf::Vector2f(120, 120) });
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
        case BPText:
            AddComponent<Text>(entity_id, {});
            AddComponent<DrawPriority>(entity_id, { 101 });
            AddComponent<Shader>(entity_id, { "", "shaders\\scroll.frag", {}, {}, {} });
            AddComponent<Position>(entity_id, {});
            AddComponent<Tag>(entity_id, {"BPText"});
            break;
        default:
            assert(false);
    }
    return entity_id;
}