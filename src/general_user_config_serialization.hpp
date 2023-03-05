
    #pragma once
    #include <string>
    #include "general_user_config.hpp"
    #include "string_parsing_utils.hpp"
    

void SerializeComponent(const GeneralConfig* c, std::string& str_rep)
{
	str_rep += "GeneralConfig{";
	str_rep += "fullscreen=";
	str_rep += ToString(c->fullscreen);
	str_rep += ";";
	str_rep += "limit_fps_to_60=";
	str_rep += ToString(c->limit_fps_to_60);
	str_rep += "}";
}

void DeserializeComponent(GeneralConfig* c, const std::string& str_rep)
{
    std::vector<std::string> variables = SplitString(str_rep, ";");
    for (auto variable : variables)
    {
        std::vector<std::string> statement_parts = SplitString(variable, "=");

        if (statement_parts[0] == "fullscreen")
        {
            FromString(c->fullscreen, statement_parts[1]);
        }

        if (statement_parts[0] == "limit_fps_to_60")
        {
            FromString(c->limit_fps_to_60, statement_parts[1]);
        }
	}
}

