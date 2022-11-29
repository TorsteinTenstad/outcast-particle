from os import listdir
from cpp_blueprints import get_blueprints as get_cpp_blueprints, ClassType
from serialization import gen_level_serialization
import json

path_components = "../src/components/"
path_blueprints = "../src/blueprints/"

data = {
    "blueprints": {},
    "components": {}
}

for (path, content, class_type) in [
    (path_blueprints, "blueprints", ClassType.blueprint),
        (path_components, "components", ClassType.component)]:
    for filename in listdir(path):
        if filename.endswith("pp"):
            get_cpp_blueprints(path + filename, class_type, data[content])

print(json.dumps(data, indent=2))

with open("../src/level_serialization.cpp", "w") as file:
    file.write(gen_level_serialization(data))
