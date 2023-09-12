
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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "Tag{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "Position{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "Charge{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "Velocity{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "WidthAndHeight{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "Player{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
void SerializeComponent(const TogglingLaser* c, std::string& str_rep)
{
	str_rep += "TogglingLaser{";
	str_rep += "period_s=";
	str_rep += ToString(c->period_s);
	str_rep += ";";
	str_rep += "duty_cycle=";
	str_rep += ToString(c->duty_cycle);
	str_rep += ";";
	str_rep += "phase=";
	str_rep += ToString(c->phase);
	str_rep += "}";
}

void DeserializeComponent(TogglingLaser* c, const std::string& entity_str_rep)
{
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "TogglingLaser{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "period_s")
        {
            FromString(c->period_s, statement_parts[1]);
        }
        else if (statement_parts[0] == "duty_cycle")
        {
            FromString(c->duty_cycle, statement_parts[1]);
        }
        else if (statement_parts[0] == "phase")
        {
            FromString(c->phase, statement_parts[1]);
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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "Collision{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "ElectricField{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "MagneticField{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
void SerializeComponent(const TextPopupSpawner* c, std::string& str_rep)
{
	str_rep += "TextPopupSpawner{";
	str_rep += "content=";
	str_rep += ToString(c->content);
	str_rep += "}";
}

void DeserializeComponent(TextPopupSpawner* c, const std::string& entity_str_rep)
{
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "TextPopupSpawner{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

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
    f << "name=" << name << ";grid_size_id=" << ToString(grid_size_id) << ";editable=" << ToString(editable) << ";music_path=" << ToString(music_path) << "\n";

    std::string entity_string;
    for (auto& [entity, tag] : GetEntitiesWith<Tag>())
    {
        if (HasComponent<NotSerialized>(entity)) { continue; }
        entity_string += entity.ToString() + ":";

        if (tag->tag == "BPEditableEntity")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
        }
        
        if (tag->tag == "BPStaticParticle")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<Charge>(entity), entity_string);
        }
        
        if (tag->tag == "BPMovingParticle")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<Charge>(entity), entity_string);
            SerializeComponent(GetComponent<Velocity>(entity), entity_string);
        }
        
        if (tag->tag == "BPBlackHole")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity), entity_string);
        }
        
        if (tag->tag == "BPPlayer")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<Charge>(entity), entity_string);
            SerializeComponent(GetComponent<Velocity>(entity), entity_string);
            SerializeComponent(GetComponent<Player>(entity), entity_string);
        }
        
        if (tag->tag == "BPLaser")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity), entity_string);
            SerializeComponent(GetComponent<TogglingLaser>(entity), entity_string);
        }
        
        if (tag->tag == "BPCoin")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
        }
        
        if (tag->tag == "BPWall")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<Collision>(entity), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity), entity_string);
        }
        
        if (tag->tag == "BPGoal")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
        }
        
        if (tag->tag == "BPElectricField")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<ElectricField>(entity), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity), entity_string);
        }
        
        if (tag->tag == "BPMagneticField")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<MagneticField>(entity), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity), entity_string);
        }
        
        if (tag->tag == "BPTextPopupSpawner")
        {
            SerializeComponent(GetComponent<Tag>(entity), entity_string);
            SerializeComponent(GetComponent<Position>(entity), entity_string);
            SerializeComponent(GetComponent<WidthAndHeight>(entity), entity_string);
            SerializeComponent(GetComponent<TextPopupSpawner>(entity), entity_string);
        }
        
        f << entity_string << "\n";
        entity_string.clear();
    }
}

Error Level::LoadFromFile(std::string savefile_path)
{
	Error err;
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
			err += FromString(name, property_statement_parts[1]);
		}
		else if (property_statement_parts[0] == "grid_size_id")
		{
			err += FromString(grid_size_id, property_statement_parts[1]);
		}
		else if (property_statement_parts[0] == "editable")
		{
			err += FromString(editable, property_statement_parts[1]);
		}
        else if (property_statement_parts[0] == "music_path")
        {
            err += FromString(music_path, property_statement_parts[1]);
        }
		else
		{
			assert(property_statement_parts[0].length() == 0);
		}
	}

    while (getline(f, line))
    {
		std::optional<std::string> id_opt = GetSubstrBetween(line, std::nullopt, ":");
		std::optional<std::string> tag_opt = GetSubstrBetween(line, "Tag{tag=", "}");
		if (!id_opt.has_value() || !tag_opt.has_value() || !IsNumeric(id_opt.value()))
		{
            err += ERROR;
			continue;
		}
		std::string id = id_opt.value();
		std::string tag = tag_opt.value();

		Entity entity(std::stoull(id));
        NotifyObservers(entity);

        AddComponent<Tag>(entity)->tag = tag;

        if (tag == "BPEditableEntity")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            DeserializeComponent(AddComponent<Position>(entity),line);
        }
        
        if (tag == "BPStaticParticle")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "particle.frag").string(), {}, {}, {} });
            AddComponent<DrawPriority>(entity, { 10 });
            AddComponent<ChargeDependentDrawInfo>(entity, {});
            AddComponent<Radius>(entity, { 108 });
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<Charge>(entity),line);
        }
        
        if (tag == "BPMovingParticle")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "particle.frag").string(), {}, {}, {} });
            AddComponent<DrawPriority>(entity, { 11 });
            AddComponent<ChargeDependentDrawInfo>(entity, {});
            AddComponent<Radius>(entity, { 108 });
            AddComponent<Acceleration>(entity, {});
            AddComponent<ReceivedForces>(entity, {});
            AddComponent<Mass>(entity, {});
            AddComponent<Intersection>(entity, {});
            AddComponent<Collision>(entity, {});
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<Charge>(entity),line);
            DeserializeComponent(AddComponent<Velocity>(entity),line);
        }
        
        if (tag == "BPBlackHole")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<DrawPriority>(entity, { 1 });
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "black_hole.frag").string(), {}, {}, {} });
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity),line);
        }
        
        if (tag == "BPPlayer")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<Radius>(entity, { 108 });
            AddComponent<Acceleration>(entity, {});
            AddComponent<ReceivedForces>(entity, {});
            AddComponent<Trail>(entity, {});
            AddComponent<Mass>(entity, {});
            AddComponent<Intersection>(entity, {});
            AddComponent<Collision>(entity, {});
            AddComponent<Children>(entity, {});
            AddComponent<DrawPriority>(entity, { 12 });
            AddComponent<Shader>(entity, { (SHADERS_DIR / "player.vert").string(), (SHADERS_DIR / "particle.frag").string(), {}, {}, {} });
            AddComponent<SoundInfo>(entity, { { { DEFAULT, (SOUNDS_DIR / "wav.wav").string() }, { TO_NEUTRAL, (SOUNDS_DIR / "to_neutral.wav").string() }, { FROM_NEUTRAL, (SOUNDS_DIR / "from_neutral.wav").string() } } });
            AddComponent<Face>(entity, {});
            AddComponent<ForceVisualization>(entity, {});
            AddComponent<PlayerBehaviors>(entity, {});
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<Charge>(entity),line);
            DeserializeComponent(AddComponent<Velocity>(entity),line);
            DeserializeComponent(AddComponent<Player>(entity),line);
        }
        
        if (tag == "BPLaser")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, { 60 });
            AddComponent<Laser>(entity, {});
            AddComponent<DrawPriority>(entity, { 3 });
            AddComponent<FillColor>(entity, {});
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity),line);
            DeserializeComponent(AddComponent<TogglingLaser>(entity),line);
        }
        
        if (tag == "BPCoin")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawPriority>(entity, { 5 });
            AddComponent<Coin>(entity, {});
            AddComponent<SegmentedGlowEffect>(entity, {});
            AddComponent<Children>(entity, {});
            AddComponent<Radius>(entity, { 60 });
            AddComponent<SoundInfo>(entity, { { { DEFAULT, (SOUNDS_DIR / "coin.wav").string() } } });
            DeserializeComponent(AddComponent<Position>(entity),line);
        }
        
        if (tag == "BPWall")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<Wall>(entity, {});
            AddComponent<DrawPriority>(entity, { 4 });
            AddComponent<SoundInfo>(entity, { { { DEFAULT, (SOUNDS_DIR / "wall_3.wav").string() } } });
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<Collision>(entity),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity),line);
        }
        
        if (tag == "BPGoal")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "wormhole.frag").string(), {}, {}, {} });
            AddComponent<DrawPriority>(entity, { 2 });
            AddComponent<Goal>(entity, {});
            AddComponent<Wormhole>(entity, {});
            AddComponent<Mass>(entity, {});
            AddComponent<SoundInfo>(entity, { { { DEFAULT, (SOUNDS_DIR / "happy_transition.wav").string() } } });
            AddComponent<Radius>(entity, { 240 });
            DeserializeComponent(AddComponent<Position>(entity),line);
        }
        
        if (tag == "BPElectricField")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<DrawPriority>(entity, { 1 });
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "electric_field.frag").string(), {}, {}, {} });
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<ElectricField>(entity),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity),line);
        }
        
        if (tag == "BPMagneticField")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<DrawPriority>(entity, { 1 });
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "magnetic_field.frag").string(), {}, {}, {} });
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<MagneticField>(entity),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity),line);
        }
        
        if (tag == "BPTextPopupSpawner")
        {
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, { (TEXTURES_DIR / "transparent.png").string(), false, 0 });
            AddComponent<DrawPriority>(entity, { 2 });
            DeserializeComponent(AddComponent<Position>(entity),line);
            DeserializeComponent(AddComponent<WidthAndHeight>(entity),line);
            DeserializeComponent(AddComponent<TextPopupSpawner>(entity),line);
        }
        
    }
    return err;
}

Entity Level::AddBlueprint(std::string blueprint_tag){
    return AddBlueprint(ToBlueprintEnum(blueprint_tag));
}

Entity Level::AddBlueprint(Blueprint blueprint)
{
    Entity entity = CreateEntity();
    switch (blueprint){
        case BPEditableEntity:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<Tag>(entity, {"BPEditableEntity"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            break;
        case BPStaticParticle:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "particle.frag").string(), {}, {}, {} });
            AddComponent<DrawPriority>(entity, { 10 });
            AddComponent<ChargeDependentDrawInfo>(entity, {});
            AddComponent<Radius>(entity, { 108 });
            AddComponent<Tag>(entity, {"BPStaticParticle"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<Charge>(entity, { 10000 });
            break;
        case BPMovingParticle:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "particle.frag").string(), {}, {}, {} });
            AddComponent<DrawPriority>(entity, { 11 });
            AddComponent<ChargeDependentDrawInfo>(entity, {});
            AddComponent<Radius>(entity, { 108 });
            AddComponent<Acceleration>(entity, {});
            AddComponent<ReceivedForces>(entity, {});
            AddComponent<Mass>(entity, {});
            AddComponent<Intersection>(entity, {});
            AddComponent<Collision>(entity, {});
            AddComponent<Tag>(entity, {"BPMovingParticle"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<Charge>(entity, { 10000 });
            AddComponent<Velocity>(entity, { sf::Vector2f(0, 0) });
            break;
        case BPBlackHole:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<DrawPriority>(entity, { 1 });
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "black_hole.frag").string(), {}, {}, {} });
            AddComponent<Tag>(entity, {"BPBlackHole"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity, { sf::Vector2f(240, 240) });
            break;
        case BPPlayer:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<Radius>(entity, { 108 });
            AddComponent<Acceleration>(entity, {});
            AddComponent<ReceivedForces>(entity, {});
            AddComponent<Trail>(entity, {});
            AddComponent<Mass>(entity, {});
            AddComponent<Intersection>(entity, {});
            AddComponent<Collision>(entity, {});
            AddComponent<Children>(entity, {});
            AddComponent<DrawPriority>(entity, { 12 });
            AddComponent<Shader>(entity, { (SHADERS_DIR / "player.vert").string(), (SHADERS_DIR / "particle.frag").string(), {}, {}, {} });
            AddComponent<SoundInfo>(entity, { { { DEFAULT, (SOUNDS_DIR / "wav.wav").string() }, { TO_NEUTRAL, (SOUNDS_DIR / "to_neutral.wav").string() }, { FROM_NEUTRAL, (SOUNDS_DIR / "from_neutral.wav").string() } } });
            AddComponent<Face>(entity, {});
            AddComponent<ForceVisualization>(entity, {});
            AddComponent<PlayerBehaviors>(entity, {});
            AddComponent<Tag>(entity, {"BPPlayer"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<Charge>(entity, { 10000 });
            AddComponent<Velocity>(entity, { sf::Vector2f(0, 0) });
            AddComponent<Player>(entity, {});
            break;
        case BPLaser:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, { 60 });
            AddComponent<Laser>(entity, {});
            AddComponent<DrawPriority>(entity, { 3 });
            AddComponent<FillColor>(entity, {});
            AddComponent<Tag>(entity, {"BPLaser"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity, { sf::Vector2f(120, 60) });
            AddComponent<TogglingLaser>(entity, {});
            break;
        case BPCoin:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawPriority>(entity, { 5 });
            AddComponent<Coin>(entity, {});
            AddComponent<SegmentedGlowEffect>(entity, {});
            AddComponent<Children>(entity, {});
            AddComponent<Radius>(entity, { 60 });
            AddComponent<SoundInfo>(entity, { { { DEFAULT, (SOUNDS_DIR / "coin.wav").string() } } });
            AddComponent<Tag>(entity, {"BPCoin"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            break;
        case BPWall:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<Wall>(entity, {});
            AddComponent<DrawPriority>(entity, { 4 });
            AddComponent<SoundInfo>(entity, { { { DEFAULT, (SOUNDS_DIR / "wall_3.wav").string() } } });
            AddComponent<Tag>(entity, {"BPWall"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<Collision>(entity, { 0.2, 75 });
            AddComponent<WidthAndHeight>(entity, { sf::Vector2f(120, 120) });
            break;
        case BPGoal:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "wormhole.frag").string(), {}, {}, {} });
            AddComponent<DrawPriority>(entity, { 2 });
            AddComponent<Goal>(entity, {});
            AddComponent<Wormhole>(entity, {});
            AddComponent<Mass>(entity, {});
            AddComponent<SoundInfo>(entity, { { { DEFAULT, (SOUNDS_DIR / "happy_transition.wav").string() } } });
            AddComponent<Radius>(entity, { 240 });
            AddComponent<Tag>(entity, {"BPGoal"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            break;
        case BPElectricField:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<DrawPriority>(entity, { 1 });
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "electric_field.frag").string(), {}, {}, {} });
            AddComponent<Tag>(entity, {"BPElectricField"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<ElectricField>(entity, { sf::Vector2f(0, 0.25) });
            AddComponent<WidthAndHeight>(entity, { sf::Vector2f(240, 240) });
            break;
        case BPMagneticField:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, {});
            AddComponent<DrawPriority>(entity, { 1 });
            AddComponent<Shader>(entity, { "", (SHADERS_DIR / "magnetic_field.frag").string(), {}, {}, {} });
            AddComponent<Tag>(entity, {"BPMagneticField"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<MagneticField>(entity, { 0.1 });
            AddComponent<WidthAndHeight>(entity, { sf::Vector2f(240, 240) });
            break;
        case BPTextPopupSpawner:
            AddComponent<ReceivesButtonEvents>(entity, {});
            AddComponent<Editable>(entity, {});
            AddComponent<DrawInfo>(entity, { (TEXTURES_DIR / "transparent.png").string(), false, 0 });
            AddComponent<DrawPriority>(entity, { 2 });
            AddComponent<Tag>(entity, {"BPTextPopupSpawner"});
            AddComponent<Position>(entity, { sf::Vector2f(0, 0) });
            AddComponent<WidthAndHeight>(entity, { sf::Vector2f(120, 120) });
            AddComponent<TextPopupSpawner>(entity, { "ipsum lorem" });
            break;
        default:
            assert(false);
    }
    return entity;
}