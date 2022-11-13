from os import listdir
from cpp_blueprints import get_blueprints as get_cpp_blueprints
from serialization import gen_level_serialization
import json

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

print(json.dumps(data, indent=2))

with open("level_serialization.cpp", "w") as file:
	file.write(gen_level_serialization(data))
