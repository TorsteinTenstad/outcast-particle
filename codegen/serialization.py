
def gen_level_serialization(data):
	cpp = """
#pragma once
#include "PCH.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"
/*
Warning!
Code is generated.
Changes will be overwritten.
*/
"""
	cpp += gen_components(data["components"])
	cpp += gen_save_to_folder(data["blueprints"])
	cpp += gen_load_from_file(data["blueprints"])
	return cpp

def gen_components(data):
	cpp = "\n"
	for component in data:
		cpp += f"""
void SerializeComponent({component} c, std::string& str_rep)
{{
"""
		cpp += "\tstr_rep += \"" + component + "{\";\n"
		attributes = data[component].get("explicit", [])
		for i, attribute in enumerate(attributes):
			if i > 0:
				cpp += f"\tstr_rep += \";\";\n"
			cpp += f"\tstr_rep += \"{attribute}=\";\n"
			cpp += f"\tstr_rep += ToString(c.{attribute});\n"
		cpp += "\tstr_rep += \"}\";\n"
		cpp += "}\n"

		cpp += f"""
void DeserializeComponent({component}& c, std::string str_rep)
{{
	std::vector<std::string> variables = SplitString(str_rep, ";");
	for (auto variable : variables)
	{{
		std::vector<std::string> statement_parts = SplitString(variable, "=");
"""
		for attribute in attributes:
			cpp += f"""
		if (statement_parts[0] == "{attribute}")
		{{
			FromString(c.{attribute}, statement_parts[1]);
		}}
"""
		cpp += "\t}\n}\n"

	return cpp + "\n"

def gen_save_to_folder(data):
	start = """
void Level::SaveToFile(std::string savefile_path)
{
	std::ofstream f(savefile_path);

	std::map<int, Tag>& tags = GetComponent<Tag>();
	std::string entity_string;
	for (auto& [entity_id, tag_component] : tags)
	{
		std::string tag = tag_component.tag;
		f << "\"" << tag << "\":";
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
		if (tag == "{tag}")
		{{"""
		for component in blueprint.get("explicit", []):
			body += f"""
			SerializeComponent(GetComponent<{component}>()[entity_id], entity_string);"""
		body += """
		}
		"""
	return start + body + end

def gen_load_from_file(data):
	start = """
void Level::LoadFromFile(std::string savefile_path)
{
	for (auto& [_, component_map_variant] : components_)
	{
		std::visit([](auto& component_map) { component_map.clear(); }, component_map_variant);
	}

	std::ifstream f(savefile_path);
	std::string line;
	while (getline(f, line))
	{
		int entity_id = CreateEntityId();
		std::string tag = GetSubstrBetween(line, "\\\"", "\\\"");
		GetComponent<Tag>()[entity_id].tag = tag;
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
			body += f"""
			GetComponent<{component}>()[entity_id] = {value}"""
		for component in blueprint.get("explicit", []):
			body += f"""
			DeserializeComponent(GetComponent<{component}>()[entity_id],
				GetSubstrBetween(line, "{component}{{", "}}"));"""
		body += """
		}
		"""
	return start + body + end
