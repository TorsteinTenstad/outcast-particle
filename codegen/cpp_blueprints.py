import re
from blueprints import handle_all_extends, merge


class ClassType:
    blueprint = 0
    component = 1


def get_blueprints(path, class_type, data={}):
    if not path.endswith("pp"):
        return {}
    with open(path, 'r') as file:
        merge(cpp_to_dict(file.read(), class_type), data)
        handle_all_extends(data)


def cpp_to_dict(cpp, class_type):
    cpp_classes = cpp.replace("public:", "").replace(
        "public", "").split("\nclass ")
    cpp_classes.pop(0)
    classes = {}
    for cpp_class in cpp_classes:
        declaration = cpp_class[:cpp_class.index("{")]
        tag, extends = (striplist(declaration.split(":"))
                        if ":" in declaration
                        else (declaration.strip(), None))
        class_ = new_class(tag, class_type)
        if extends:
            class_["extends"] = extends
        assignments = re.findall(
            "[\[\w+\]]*[\n\t ]*\w+ \w+ = .*\;", cpp_class)
        for assignment in assignments:
            left, right = striplist(assignment.split("="))
            above, left = left.split("\n") if "\n" in left else ("", left)
            decorators = [d[1:len(d)-1].lower()
                          for d in re.findall("\[\w+\]", above)]
            add_assignment(class_, left, right, class_type, decorators)
        empty_assignments = re.findall(
            "[\[\w+\]]*[\n\t ]*\w+ \w+\;", cpp_class)
        for assignment in empty_assignments:
            left = assignment[:-1]
            add_assignment(class_, left, None, class_type)
        classes[tag] = class_
    return classes


def add_assignment(class_, left, right, class_type, decorators=[]):
    type_name, var_name = striplist(left.split(" "))
    if class_type == ClassType.blueprint:
        if "serialize" in decorators:
            class_["explicit"][type_name] = right
        else:
            class_["implicit"][type_name] = right
    else:
        class_["explicit"][var_name] = right


def striplist(l):
    return [x.strip() for x in l]


def new_class(tag, class_type):
    class_ = {
        "explicit": {},
        "implicit": {}
    }
    if class_type == ClassType.blueprint:
        class_["explicit"]["Tag"] = f"{{\"{tag}\"}};"
    return class_
