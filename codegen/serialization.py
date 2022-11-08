
def gen_level_serialization(data):
	cpp = """
#include \"level.hpp\"
/*
Warning!
Code is generated.
Changes will be overwritten.
*/
"""
	cpp += gen_components(data["components"])
	cpp += gen_save_to_folder(data["blueprints"])
	cpp += gen_load_from_folder(data["blueprints"])
	return cpp

def gen_components(data):
	cpp = ""
	for component in data:
		cpp += f"\nstatic void SerializeComponent({component} c);"
		cpp += f"\nstatic void DeserializeComponent({component}& c);"
	return cpp + "\n"

def gen_save_to_folder(data):
	start = """
void Level::SaveToFolder(std::string folder_path)
{
	std::map<int, Tag>& tags = GetComponent<Tag>();
	for (auto& [entity_id, tag_component] : tags)
	{
		std::string tag = tag_component.tag;
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
		for component in blueprint["components"].get("explicit", []):
			body += f"""
			SerializeComponent(GetComponent<{component}>()[entity_id]);"""
		body += """
		}
		"""
	return start + body + end

def gen_load_from_folder(data):
	start = """
void Level::LoadFromFolder(std::string folder_path)
{
	for (auto& [_, component_map_variant] : components_)
	{
		std::visit([](auto& component_map) { component_map.clear(); }, component_map_variant);
	}

	std::vector<std::string> entity_tags_read_from_file;

	for (auto tag : entity_tags_read_from_file)
	{
		int entity_id = CreateEntityId();
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
		for (component, value) in blueprint["components"].get(
				"implicit", {}).items():
			body += f"""
			GetComponent<{component}>()[entity_id] = {value}"""
		for component in blueprint["components"].get("explicit", []):
			body += f"""
			DeserializeComponent(GetComponent<{component}>()[entity_id]);"""
		body += """
		}
		"""
	return start + body + end
