import re
import copy

def get_blueprints(path):
	if not path.endswith(".cpp"): return {}
	with open(path, 'r') as file:
		data = cpp_to_dict(file.read())
		handle_tags(data)
		handle_all_extends(data)
		return data

def cpp_to_dict(cpp):
	cpp_blueprints = cpp.split("\nclass ")
	cpp_blueprints.pop(0)
	blueprints = {}
	for cpp_blueprint in cpp_blueprints:
		declaration = cpp_blueprint[:cpp_blueprint.index("{")]
		tag, extends = (striplist(declaration.split(":"))
			if ":" in declaration
			else (declaration.strip(), None))
		blueprint = new_blueprint(tag)
		if extends:
			blueprint["extends"] = extends
		assignments = re.findall(
			"[\[\w+\]]*[\n\t ]*\w+ \w+ = \{.*\}\;", cpp_blueprint)
		for assignment in assignments:
			left, right = striplist(assignment.split("="))
			above, left = left.split("\n") if "\n" in left else ("", left)
			decorators = [d[1:len(d)-1].lower()
				for d in re.findall("\[\w+\]", above)]
			component, _ = striplist(left.split(" "))
			if "serialize" in decorators:
				blueprint["components"]["explicit"][component] = right
			else:
				blueprint["components"]["implicit"][component] = right
		blueprints[tag] = blueprint
	return blueprints

def striplist(l):
	return [x.strip() for x in l]

def new_blueprint(tag):
	return {
		"components": {
			"explicit": {
				"Tag": tag
			},
			"implicit": {}
		}
	}

def handle_tags(data):
	for (tag, body) in data.items():
		if not body.get("components", None):
			body["components"] = {}
		if not body["components"].get("explicit", None):
			body["components"]["explicit"] = {}
		body["components"]["explicit"]["Tag"] = tag

def handle_all_extends(data):
	for tag in data:
		handle_extends(data, tag)

def handle_extends(data, tag):
	body = data[tag]
	parent_tag = body.get("extends", None)
	if not parent_tag:
		return
	handle_extends(data, parent_tag)
	del body["extends"]
	data[tag] = merge(body, copy.deepcopy(data[parent_tag]))

def merge(source, destination):
    for key, value in source.items():
        if isinstance(value, dict):
            node = destination.setdefault(key, {})
            merge(value, node)
        else:
            destination[key] = value
    return destination
