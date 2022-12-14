from os import listdir
from cpp_blueprints import get_blueprints as get_cpp_blueprints, ClassType
from serialization import gen_level_serialization, gen_components
import json

path_components = "src/components/"
path_blueprints = "src/blueprints/"

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

with open("src/level_serialization.cpp", "w") as file:
    file.write(gen_level_serialization(data))


controls_config_data = {}
get_cpp_blueprints("src/controls_config.hpp",
                   ClassType.component, controls_config_data)
print(json.dumps(controls_config_data, indent=2))

cpp = """
#pragma once
#include "PCH.hpp"
#include "controls_config.hpp"
#include "string_parsing_utils.hpp"
"""
cpp += gen_components(controls_config_data, ["KeyConfig"])
with open("src/controls_config_serialization.hpp", "w") as file:
    file.write(cpp)
