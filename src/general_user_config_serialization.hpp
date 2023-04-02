
    #pragma once
    #include <cassert>
    #include <string>
    #include "general_user_config.hpp"
    #include "utils/string_parsing.hpp"
    

void SerializeComponent(const GeneralConfig* c, std::string& str_rep)
{
	str_rep += "GeneralConfig{";
	str_rep += "fullscreen=";
	str_rep += ToString(c->fullscreen);
	str_rep += ";";
	str_rep += "forces_are_visualized=";
	str_rep += ToString(c->forces_are_visualized);
	str_rep += ";";
	str_rep += "use_ready_mode=";
	str_rep += ToString(c->use_ready_mode);
	str_rep += ";";
	str_rep += "show_timer=";
	str_rep += ToString(c->show_timer);
	str_rep += ";";
	str_rep += "sound_volume=";
	str_rep += ToString(c->sound_volume);
	str_rep += "}";
}

void DeserializeComponent(GeneralConfig* c, const std::string& entity_str_rep)
{
    std::string component_str = GetSubstrBetween(entity_str_rep, "GeneralConfig{", "}");
    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "fullscreen")
        {
            FromString(c->fullscreen, statement_parts[1]);
        }
        else if (statement_parts[0] == "forces_are_visualized")
        {
            FromString(c->forces_are_visualized, statement_parts[1]);
        }
        else if (statement_parts[0] == "use_ready_mode")
        {
            FromString(c->use_ready_mode, statement_parts[1]);
        }
        else if (statement_parts[0] == "show_timer")
        {
            FromString(c->show_timer, statement_parts[1]);
        }
        else if (statement_parts[0] == "sound_volume")
        {
            FromString(c->sound_volume, statement_parts[1]);
        }
        else {
            assert(false);
        }
    }
}
