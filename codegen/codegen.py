from os import listdir
from cpp_blueprints import get_blueprints as get_cpp_blueprints
from yaml_blueprints import get_blueprints as get_yaml_blueprints
from yaml_blueprints import stringify_blueprints

path_components = "../src/components/"
path_blueprints = "../src/blueprints/"

for filename in listdir(path_blueprints):
    if filename.endswith(".yaml"):
        data = get_yaml_blueprints(path_blueprints + filename)
    if filename.endswith(".cpp"):
        data = get_cpp_blueprints(path_blueprints + filename)
    print(stringify_blueprints(data))
