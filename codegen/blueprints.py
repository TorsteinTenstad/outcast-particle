import copy

def handle_tags(data):
	for (tag, body) in data.items():
		if not body.get("components", None):
			body["components"] = {}
		if not body["components"].get("explicit", None):
			body["components"]["explicit"] = {}
		body["components"]["explicit"]["Tag"] = tag

def handle_all_extends(data):
	for tag in data:
		handle_extends(data, tag)

def handle_extends(data, tag):
	body = data[tag]
	parent_tag = body.get("extends", None)
	if not parent_tag:
		return
	handle_extends(data, parent_tag)
	del body["extends"]
	data[tag] = merge(body, copy.deepcopy(data[parent_tag]))

def merge(source, destination):
    for key, value in source.items():
        if isinstance(value, dict):
            node = destination.setdefault(key, {})
            merge(value, node)
        else:
            destination[key] = value
    return destination
