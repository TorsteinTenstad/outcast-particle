from os import listdir
from cpp_blueprints import get_blueprints as get_cpp_blueprints, ClassType
from serialization import gen_level_serialization, gen_components, gen_blueprint_enum
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


with open("src/blueprint.hpp", "w") as file:
    file.write(gen_blueprint_enum(data["blueprints"]))


def serialize_single_class(class_definition_filepath, output_filepath, classname):
    data = {}
    get_cpp_blueprints(class_definition_filepath,
                       ClassType.component, data)
    print(json.dumps(data, indent=2))
    filename = "/".join(class_definition_filepath.split("/")[1:])

    cpp = f"""
    #pragma once
    #include <cassert>
    #include <string>
    #include "{filename}"
    #include "utils/string_parsing.hpp"
    """
    cpp += gen_components(data, [classname])
    with open(output_filepath, "w") as file:
        file.write(cpp)


serialize_single_class("src/controls_config.hpp",
                       "src/controls_config_serialization.hpp", "KeyConfig")
serialize_single_class("src/general_user_config.hpp",
                       "src/general_user_config_serialization.hpp", "GeneralConfig")
