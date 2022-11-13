import copy

def handle_all_extends(data):
	for tag in data:
		handle_extends(data, tag)

def handle_extends(data, tag):
	if not tag in data:
		print(f"Warning: handle_extends {tag}")
		return
	body = data[tag]
	parent_tag = body.get("extends", None)
	if not parent_tag:
		return
	parent_data = data.get(parent_tag, None)
	if not parent_data:
		print(f"Warning: handle_extends {parent_tag}")
		return
	handle_extends(data, parent_tag)
	del body["extends"]
	data[tag] = merge(body, copy.deepcopy(parent_data))

def merge(source, destination):
    for key, value in source.items():
        if isinstance(value, dict):
            node = destination.setdefault(key, {})
            merge(value, node)
        else:
            destination[key] = value
    return destination
