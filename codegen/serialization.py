
def gen_level_serialization(data):
    cpp = """
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
"""
    explicit_components = []
    for blueprint in data["blueprints"].values():
        for component_tag in blueprint.get("explicit", {}).keys():
            if component_tag not in explicit_components:
                explicit_components.append(component_tag)

    cpp += gen_components(data["components"], explicit_components, True)
    cpp += gen_save_to_file(data["blueprints"])
    cpp += gen_load_from_file(data["blueprints"])
    cpp += gen_add_blueprint(data["blueprints"])
    return cpp


def gen_components(data, components_to_generate, allow_unmatched_statements=False):
    cpp = "\n"
    for component in components_to_generate:
        cpp += f"""
void SerializeComponent(const {component}* c, std::string& str_rep)
{{
"""
        cpp += "\tstr_rep += \"" + component + "{\";\n"
        attributes = data[component].get("explicit", [])
        for i, attribute in enumerate(attributes):
            if i > 0:
                cpp += f"\tstr_rep += \";\";\n"
            cpp += f"\tstr_rep += \"{attribute}=\";\n"
            cpp += f"\tstr_rep += ToString(c->{attribute});\n"
        cpp += "\tstr_rep += \"}\";\n"
        cpp += "}\n"

        cpp += f"""
void DeserializeComponent({component}* c, const std::string& entity_str_rep)
{{
    std::optional<std::string> component_str_opt = GetSubstrBetween(entity_str_rep, "{component}{{", "}}");
    if (!component_str_opt.has_value()) {{ return; }}
	std::string component_str = component_str_opt.value();

    std::vector<std::string> variables = SplitString(component_str, ";");
    for (auto variable : variables)
    {{
        std::vector<std::string> statement_parts = SplitString(variable, "=");
"""
        for i, attribute in enumerate(attributes):
            cpp += f"""
        {"else " if i else ""}if (statement_parts[0] == "{attribute}")
        {{
            FromString(c->{attribute}, statement_parts[1]);
        }}"""
        if not allow_unmatched_statements:
            cpp += """
        else {
            assert(false);
        }"""
        cpp += """
    }
}"""

    return cpp + "\n"


def gen_save_to_file(data):
    start = """
void Level::SaveToFile(std::string savefile_path)
{
    std::ofstream f(savefile_path);
    f << "name=" << name << ";grid_size_id=" << ToString(grid_size_id) << ";editable=" << ToString(editable) << "\\n";

    std::string entity_string;
    for (auto& [entity, tag] : GetEntitiesWith<Tag>())
    {
        if (HasComponent<NotSerialized>(entity)) { continue; }
        entity_string += entity.ToString() + ":";
"""
    end = """
        f << entity_string << "\\n";
        entity_string.clear();
    }
}
"""
    body = ""
    for (tag, blueprint) in data.items():
        body += f"""
        if (tag->tag == "{tag}")
        {{"""
        for component in blueprint.get("explicit", []):
            body += f"""
            SerializeComponent(GetComponent<{component}>(entity), entity_string);"""
        body += """
        }
        """
    return start + body + end


def gen_load_from_file(data):
    start = """
Error_t Level::LoadFromFile(std::string savefile_path)
{
	Error_t err;
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
"""
    end = """
    }
    return err;
}
"""
    body = ""
    for (tag, blueprint) in data.items():
        body += f"""
        if (tag == "{tag}")
        {{"""
        for (component, value) in blueprint.get(
                "implicit", {}).items():
            if component == 'Tag':
                continue
            body += f"""
            AddComponent<{component}>(entity, {value.replace(';', '')});"""
        for component in blueprint.get("explicit", []):
            if component == 'Tag':
                continue
            body += f"""
            DeserializeComponent(AddComponent<{component}>(entity),line);"""
        body += """
        }
        """
    return start + body + end


def gen_add_blueprint(data):
    start = """
Entity Level::AddBlueprint(std::string blueprint_tag){
    return AddBlueprint(ToBlueprintEnum(blueprint_tag));
}

Entity Level::AddBlueprint(Blueprint blueprint)
{
    Entity entity = CreateEntity();
    switch (blueprint){"""
    end = """
        default:
            assert(false);
    }
    return entity;
}"""
    body = ""
    for (tag, blueprint) in data.items():
        body += f"""
        case {tag}:"""
        for (component, value) in blueprint.get(
                "implicit", {}).items():
            body += f"""
            AddComponent<{component}>(entity, {value.replace(';', '')});"""
        for (component, value) in blueprint.get("explicit", {}).items():
            body += f"""
            AddComponent<{component}>(entity, {value.replace(';', '')});"""
        body += f"""
            break;"""
    return start + body + end


def gen_blueprint_enum_hpp(data):
    enum_entries = ",\n\t".join(
        [f"{tag} = {i}" for i, tag in enumerate(data.keys())])
    enum_decl = f"""#pragma once
#include <string>
#include <vector>
#include <map>

enum Blueprint
{{
    {enum_entries}
}};"""

    enum_entries = ",\n\t\t".join([f"\"{tag}\"" for tag in data.keys()])
    converters = """
    
std::string ToBlueprintTag(Blueprint blueprint);
Blueprint ToBlueprintEnum(std::string blueprint_tag);
"""
    return enum_decl + converters


def gen_blueprint_enum_cpp(data):
    enum_entries = ",\n\t\t".join([f"\"{tag}\"" for tag in data.keys()])
    to_tag = f"""
#include "blueprint.hpp"

std::string ToBlueprintTag(Blueprint blueprint)
{{
    return std::vector<std::string>({{
        {enum_entries}}})[(int(blueprint))];
}}"""

    enum_entries = ",\n\t\t".join(
        [f"{{\"{tag}\", {tag}}}" for tag in data.keys()])
    to_enum = f"""

Blueprint ToBlueprintEnum(std::string blueprint_tag)
{{
    return std::map<std::string, Blueprint>({{
        {enum_entries}}}).at(blueprint_tag);
}}"""
    return to_tag + to_enum
