from os import listdir
from cpp_blueprints import get_blueprints as get_cpp_blueprints
from yaml_blueprints import get_blueprints as get_yaml_blueprints
from yaml_blueprints import stringify_blueprints
from serialization import gen_level_serialization

path_components = "../src/components/"
path_blueprints = "../src/blueprints/"

data = {
	"blueprints": {},
	"components": {}
}

for (path, content) in [
		(path_blueprints, "blueprints"),
		(path_components, "components")]:
	for filename in listdir(path):
		if filename.endswith("pp"):
			get_cpp_blueprints(
				path + filename, data[content])

print(stringify_blueprints(data))

with open("level_serialization.cpp", "w") as file:
	file.write(gen_level_serialization(data))
