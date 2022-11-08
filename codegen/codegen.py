from os import listdir
from cpp_blueprints import get_blueprints
from yaml_blueprints import stringify_blueprints

path_components = "../src/components/"
path_blueprints = "../src/blueprints/"

for filename in listdir(path_blueprints):
    data = get_blueprints(path_blueprints + filename)
    print(stringify_blueprints(data))
