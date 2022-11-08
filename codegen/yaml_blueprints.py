import yaml as ya
from yaml.loader import SafeLoader
from blueprints import handle_tags, handle_all_extends

def get_blueprints(path: str):
	if not path.endswith(".yaml"): return {}
	with open(path) as file:
		data = ya.load(file, Loader=SafeLoader)
		handle_tags(data)
		handle_all_extends(data)
		return data

def stringify_blueprints(data):
	return ya.dump(data, default_flow_style=False, sort_keys=False)
