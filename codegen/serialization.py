
def gen_level_serialization(data):
    cpp = """
#include <cassert>
#include <string>
#include <vector>
#include <fstream>

#include "level.hpp"
#include "blueprint.hpp"
#include "utils/string_parsing.hpp"

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
    Clear();

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
int Level::AddBlueprint(std::string blueprint_tag){
    return AddBlueprint(ToBlueprintEnum(blueprint_tag));
}

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


def gen_blueprint_enum_hpp(data):
    enum_entries = ",\n\t".join([f"{tag} = {i}" for i, tag in enumerate(data.keys())])
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

    enum_entries = ",\n\t\t".join([f"{{\"{tag}\", {tag}}}" for tag in data.keys()])
    to_enum = f"""

Blueprint ToBlueprintEnum(std::string blueprint_tag)
{{
    return std::map<std::string, Blueprint>({{
        {enum_entries}}}).at(blueprint_tag);
}}"""
    return to_tag + to_enum