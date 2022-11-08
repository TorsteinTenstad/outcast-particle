import re
from blueprints import handle_tags, handle_all_extends

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
