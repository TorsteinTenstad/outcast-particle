
    #pragma once
    #include <cassert>
    #include <string>
    #include "controls_config.hpp"
    #include "utils/string_parsing.hpp"
    

void SerializeComponent(const KeyConfig* c, std::string& str_rep)
{
	str_rep += "KeyConfig{";
	str_rep += "PLAYER_MOVE_LEFT=";
	str_rep += ToString(c->PLAYER_MOVE_LEFT);
	str_rep += ";";
	str_rep += "PLAYER_MOVE_RIGHT=";
	str_rep += ToString(c->PLAYER_MOVE_RIGHT);
	str_rep += ";";
	str_rep += "PLAYER_MOVE_UP=";
	str_rep += ToString(c->PLAYER_MOVE_UP);
	str_rep += ";";
	str_rep += "PLAYER_MOVE_DOWN=";
	str_rep += ToString(c->PLAYER_MOVE_DOWN);
	str_rep += ";";
	str_rep += "PLAYER_SWITCH_CHARGE=";
	str_rep += ToString(c->PLAYER_SWITCH_CHARGE);
	str_rep += ";";
	str_rep += "PLAYER_GO_NEUTRAL=";
	str_rep += ToString(c->PLAYER_GO_NEUTRAL);
	str_rep += ";";
	str_rep += "MENU=";
	str_rep += ToString(c->MENU);
	str_rep += ";";
	str_rep += "EDIT_MODE=";
	str_rep += ToString(c->EDIT_MODE);
	str_rep += ";";
	str_rep += "INCREASE_LEVEL_SIZE=";
	str_rep += ToString(c->INCREASE_LEVEL_SIZE);
	str_rep += ";";
	str_rep += "DECREASE_LEVEL_SIZE=";
	str_rep += ToString(c->DECREASE_LEVEL_SIZE);
	str_rep += ";";
	str_rep += "EDIT_MODE_FLIP_CHARGES_AND_FIELDS=";
	str_rep += ToString(c->EDIT_MODE_FLIP_CHARGES_AND_FIELDS);
	str_rep += ";";
	str_rep += "SELECT_MULTIPLE_ENTITIES=";
	str_rep += ToString(c->SELECT_MULTIPLE_ENTITIES);
	str_rep += ";";
	str_rep += "COPY_ENTITY=";
	str_rep += ToString(c->COPY_ENTITY);
	str_rep += ";";
	str_rep += "DELETE_ENTITY=";
	str_rep += ToString(c->DELETE_ENTITY);
	str_rep += ";";
	str_rep += "SNAP_TO_GRID=";
	str_rep += ToString(c->SNAP_TO_GRID);
	str_rep += ";";
	str_rep += "ALT_SENSITIVITY=";
	str_rep += ToString(c->ALT_SENSITIVITY);
	str_rep += ";";
	str_rep += "ROTATE_ENTITY=";
	str_rep += ToString(c->ROTATE_ENTITY);
	str_rep += ";";
	str_rep += "INCREMENT_HEIGHT=";
	str_rep += ToString(c->INCREMENT_HEIGHT);
	str_rep += ";";
	str_rep += "DECREMENT_HEIGHT=";
	str_rep += ToString(c->DECREMENT_HEIGHT);
	str_rep += ";";
	str_rep += "DECREMENT_WIDTH=";
	str_rep += ToString(c->DECREMENT_WIDTH);
	str_rep += ";";
	str_rep += "INCREMENT_WIDTH=";
	str_rep += ToString(c->INCREMENT_WIDTH);
	str_rep += ";";
	str_rep += "INCREMENT_VELOCITY=";
	str_rep += ToString(c->INCREMENT_VELOCITY);
	str_rep += ";";
	str_rep += "DECREMENT_VELOCITY=";
	str_rep += ToString(c->DECREMENT_VELOCITY);
	str_rep += ";";
	str_rep += "DECREMENT_VELOCITY_ANGLE=";
	str_rep += ToString(c->DECREMENT_VELOCITY_ANGLE);
	str_rep += ";";
	str_rep += "INCREMENT_VELOCITY_ANGLE=";
	str_rep += ToString(c->INCREMENT_VELOCITY_ANGLE);
	str_rep += ";";
	str_rep += "RESET_SHADERS_AND_TEXTURES=";
	str_rep += ToString(c->RESET_SHADERS_AND_TEXTURES);
	str_rep += "}";
}

void DeserializeComponent(KeyConfig* c, const std::string& entity_str_rep)
{
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "KeyConfig{", "}");
    if (!component_str_opt.has_value()) { return; }
	std::string component_str = component_str_opt.value();

    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "PLAYER_MOVE_LEFT")
        {
            FromString(c->PLAYER_MOVE_LEFT, statement_parts[1]);
        }
        else if (statement_parts[0] == "PLAYER_MOVE_RIGHT")
        {
            FromString(c->PLAYER_MOVE_RIGHT, statement_parts[1]);
        }
        else if (statement_parts[0] == "PLAYER_MOVE_UP")
        {
            FromString(c->PLAYER_MOVE_UP, statement_parts[1]);
        }
        else if (statement_parts[0] == "PLAYER_MOVE_DOWN")
        {
            FromString(c->PLAYER_MOVE_DOWN, statement_parts[1]);
        }
        else if (statement_parts[0] == "PLAYER_SWITCH_CHARGE")
        {
            FromString(c->PLAYER_SWITCH_CHARGE, statement_parts[1]);
        }
        else if (statement_parts[0] == "PLAYER_GO_NEUTRAL")
        {
            FromString(c->PLAYER_GO_NEUTRAL, statement_parts[1]);
        }
        else if (statement_parts[0] == "MENU")
        {
            FromString(c->MENU, statement_parts[1]);
        }
        else if (statement_parts[0] == "EDIT_MODE")
        {
            FromString(c->EDIT_MODE, statement_parts[1]);
        }
        else if (statement_parts[0] == "INCREASE_LEVEL_SIZE")
        {
            FromString(c->INCREASE_LEVEL_SIZE, statement_parts[1]);
        }
        else if (statement_parts[0] == "DECREASE_LEVEL_SIZE")
        {
            FromString(c->DECREASE_LEVEL_SIZE, statement_parts[1]);
        }
        else if (statement_parts[0] == "EDIT_MODE_FLIP_CHARGES_AND_FIELDS")
        {
            FromString(c->EDIT_MODE_FLIP_CHARGES_AND_FIELDS, statement_parts[1]);
        }
        else if (statement_parts[0] == "SELECT_MULTIPLE_ENTITIES")
        {
            FromString(c->SELECT_MULTIPLE_ENTITIES, statement_parts[1]);
        }
        else if (statement_parts[0] == "COPY_ENTITY")
        {
            FromString(c->COPY_ENTITY, statement_parts[1]);
        }
        else if (statement_parts[0] == "DELETE_ENTITY")
        {
            FromString(c->DELETE_ENTITY, statement_parts[1]);
        }
        else if (statement_parts[0] == "SNAP_TO_GRID")
        {
            FromString(c->SNAP_TO_GRID, statement_parts[1]);
        }
        else if (statement_parts[0] == "ALT_SENSITIVITY")
        {
            FromString(c->ALT_SENSITIVITY, statement_parts[1]);
        }
        else if (statement_parts[0] == "ROTATE_ENTITY")
        {
            FromString(c->ROTATE_ENTITY, statement_parts[1]);
        }
        else if (statement_parts[0] == "INCREMENT_HEIGHT")
        {
            FromString(c->INCREMENT_HEIGHT, statement_parts[1]);
        }
        else if (statement_parts[0] == "DECREMENT_HEIGHT")
        {
            FromString(c->DECREMENT_HEIGHT, statement_parts[1]);
        }
        else if (statement_parts[0] == "DECREMENT_WIDTH")
        {
            FromString(c->DECREMENT_WIDTH, statement_parts[1]);
        }
        else if (statement_parts[0] == "INCREMENT_WIDTH")
        {
            FromString(c->INCREMENT_WIDTH, statement_parts[1]);
        }
        else if (statement_parts[0] == "INCREMENT_VELOCITY")
        {
            FromString(c->INCREMENT_VELOCITY, statement_parts[1]);
        }
        else if (statement_parts[0] == "DECREMENT_VELOCITY")
        {
            FromString(c->DECREMENT_VELOCITY, statement_parts[1]);
        }
        else if (statement_parts[0] == "DECREMENT_VELOCITY_ANGLE")
        {
            FromString(c->DECREMENT_VELOCITY_ANGLE, statement_parts[1]);
        }
        else if (statement_parts[0] == "INCREMENT_VELOCITY_ANGLE")
        {
            FromString(c->INCREMENT_VELOCITY_ANGLE, statement_parts[1]);
        }
        else if (statement_parts[0] == "RESET_SHADERS_AND_TEXTURES")
        {
            FromString(c->RESET_SHADERS_AND_TEXTURES, statement_parts[1]);
        }
        else {
            assert(false);
        }
    }
}
