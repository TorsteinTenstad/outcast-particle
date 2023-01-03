#include "level.hpp"

template <class Component>
std::map<int, Component>& Level::GetComponent()
{
	if (components_.count(typeid(Component)) == 0)
	{
		std::map<int, Component> t_map;
		components_[typeid(Component)] = t_map;
	}
	return std::get<std::map<int, Component>>(components_[typeid(Component)]);
}

template <class OtherComponent, class... Component>
static bool IdIntersection(int component_idx, std::map<int, OtherComponent>& component_map, std::vector<std::tuple<int, Component*...>>& matching_entities)
{
	if (component_map.size() == 0)
	{
		matching_entities.clear();
		return false;
	}
	if (component_idx == 0)
	{
		for (auto& [entity_id, component_value] : component_map)
		{
			std::tuple<int, Component*...> x = {};
			std::get<int>(x) = entity_id;
			std::get<OtherComponent*>(x) = &component_value;
			matching_entities.push_back(x);
		}
	}
	else
	{
		for (auto it = matching_entities.begin(), next_it = it; it != matching_entities.end(); it = next_it)
		{
			++next_it;
			auto map_it = component_map.find(std::get<int>(*it));
			if (map_it == component_map.end())
			{
				matching_entities.erase(it);
			}
			else
			{
				std::get<OtherComponent*>(*it) = &map_it->second;
			}
		}
	}
	return true;
}

template <class... Component>
std::vector<std::tuple<int, Component*...>> Level::GetEntitiesWith()
{
	int component_idx = 0;
	std::vector<std::tuple<int, Component*...>> matching_entities = {};
	(IdIntersection<Component, Component...>(component_idx++, GetComponent<Component>(), matching_entities) && ...);
	return matching_entities;
}

template <class ResponsibleComponent>
Shader* CreateScreenwideFragmentShaderEntity(Level& level, Children* parents_children, std::string shader_path)
{
	if (parents_children->ids_owned_by_component.count(typeid(ResponsibleComponent)) == 0)
	{
		int id = level.CreateEntityId();
		parents_children->ids_owned_by_component[typeid(ResponsibleComponent)].push_back(id);
		level.GetComponent<Position>()[id].position = level.size / 2.f;
		level.GetComponent<WidthAndHeight>()[id].width_and_height = level.size;
		level.GetComponent<DrawPriority>()[id].draw_priority = 5;
		level.GetComponent<DrawInfo>()[id].image_path = "content\\textures\\transparent.png";
		level.GetComponent<Shader>()[id].fragment_shader_path = shader_path;
		level.GetComponent<Shader>()[id].float_uniforms["_time"];
	}
	std::vector<int> visualization_entities = parents_children->ids_owned_by_component[typeid(ResponsibleComponent)];
	assert(visualization_entities.size() == 1);
	int visualization_entity = visualization_entities[0];
	assert(level.GetComponent<Shader>().count(visualization_entity) > 0);
	return &level.GetComponent<Shader>()[visualization_entity];
}
