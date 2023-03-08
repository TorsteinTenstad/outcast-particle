
def gen_level_serialization(data):
    cpp = """
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
"""
    explicit_components = []
    for blueprint in data["blueprints"].values():
        for component_tag in blueprint.get("explicit", {}).keys():
            if component_tag not in explicit_components:
                explicit_components.append(component_tag)

    cpp += gen_components(data["components"], explicit_components)
    cpp += gen_save_to_file(data["blueprints"])
    cpp += gen_load_from_file(data["blueprints"])
    cpp += gen_add_blueprint(data["blueprints"])
    return cpp


def gen_components(data, components_to_generate):
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
    std::string component_str = GetSubstrBetween(entity_str_rep, "{component}{{", "}}");
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
        cpp += """
        else {{
            assert(false);
        }}
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
    for (auto& [entity_id, tag] : GetEntitiesWith<Tag>())
    {
        if (HasComponent<NotSerialized>(entity_id)){
            continue;
        }
        f << "\\"" << tag->tag << "\\":";
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
            SerializeComponent(GetComponent<{component}>(entity_id), entity_string);"""
        body += """
        }
        """
    return start + body + end


def gen_load_from_file(data):
    start = """
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
        std::string tag = GetSubstrBetween(line, "\\\"", "\\\"");
        AddComponent<Tag>(entity_id)->tag = tag;
"""
    end = """
    }
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
            AddComponent<{component}>(entity_id, {value.replace(';', '')});"""
        for component in blueprint.get("explicit", []):
            if component == 'Tag':
                continue
            body += f"""
            DeserializeComponent(AddComponent<{component}>(entity_id),line);"""
        body += """
        }
        """
    return start + body + end


def gen_add_blueprint(data):
    start = """
int Level::AddBlueprint(Blueprint blueprint)
{
    int entity_id = CreateEntityId();
    switch (blueprint){"""
    end = """
        default:
            assert(false);
    }
    return entity_id;
}"""
    body = ""
    for (tag, blueprint) in data.items():
        body += f"""
        case {tag}:"""
        for (component, value) in blueprint.get(
                "implicit", {}).items():
            body += f"""
            AddComponent<{component}>(entity_id, {value.replace(';', '')});"""
        for (component, value) in blueprint.get("explicit", {}).items():
            body += f"""
            AddComponent<{component}>(entity_id, {value.replace(';', '')});"""
        body += f"""
            break;"""
    return start + body + end


def gen_blueprint_enum(data):
    start = """#pragma once
    
enum Blueprint
{
    """
    end = """
};"""
    body = ",\n\t".join([tag for tag in data.keys()])
    return start + body + end
