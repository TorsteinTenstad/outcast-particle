#include "level.hpp"
#include "game.hpp"
#include "string_parsing_utils.hpp"
#include <functional>
#include <string>

int Level::next_available_entity_id_ = 0;

Level::Level(int level_id) :
	id(level_id)
{
}

int Level::CopyEntity(int from_id)
{
	int to_id = CreateEntityId();
	for (auto& [component_type_id, component_map_variant] : components_)
	{
		if (component_type_id == typeid(Children))
		{
			continue;
		}
		std::visit([from_id, to_id](auto& component_map) {
			if (component_map.count(from_id) != 0)
			{
				component_map[to_id] = component_map[from_id];
			} },
			component_map_variant);
	}
	return to_id;
}

void Level::DeleteEntity(int id)
{
	auto& children_map = GetComponent<Children>();
	if (children_map.count(id) > 0)
	{
		for (auto& [component_type_id, child_ids] : children_map[id].ids_owned_by_component)
		{
			for (auto& child_id : child_ids)
			{
				DeleteEntity(child_id);
			}
		}
	}
	for (auto& [_, component_map_variant] : components_)
	{
		std::visit([id](auto& component_map) {
			if (component_map.count(id) != 0)
			{
				component_map.erase(id);
			} },
			component_map_variant);
	}
}

int Level::CreateEntityId()
{
	return next_available_entity_id_++;
}

void Level::LoadFromFile()
{
	LoadFromFile(savefile_path_);
}

void Level::SaveToFile()
{
	SaveToFile(savefile_path_);
}

void Level::SaveRecordToFile(int coin_record, float completion_time_record)
{
	SaveRecordToFile(savefile_path_, coin_record, completion_time_record);
}

int Level::AddLevelButton(int level, std::function<void(void)> on_click, float pos_x, float pos_y, float width, float height, std::string image_path)
{
	int id = CreateEntityId();
	GetComponent<LevelButton>()[id].level = level;
	GetComponent<DrawInfo>()[id].image_path = image_path;
	GetComponent<DrawInfo>()[id].scale_to_fit = true;
	GetComponent<DrawPriority>()[id].draw_priority = 1;
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(width, height) };
	GetComponent<CanReceivePress>()[id] = {};
	GetComponent<Border>()[id];
	GetComponent<Button>()[id].on_click = on_click;
	GetComponent<Button>()[id].image_path = image_path;
	return id;
}

int Level::AddMenuButton(std::function<void(void)> on_click, float pos_x, float pos_y, std::string button_text)
{
	std::string image_path_suffix = "menu_wide.png";
	std::string image_path = "content\\textures_generated\\button_" + image_path_suffix;
	std::string pressed_image_path = "content\\textures_generated\\pressed_button_" + image_path_suffix;
	int id = AddBlueprint("BPButton");
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<Button>()[id].on_click = on_click;
	GetComponent<Button>()[id].image_path = image_path;
	GetComponent<Button>()[id].pressed_image_path = pressed_image_path;
	GetComponent<Text>()[id].content = button_text;
	return id;
}

int Level::AddOptionsButton(sf::Keyboard::Key* key, float pos_x, float pos_y, float width, float height, std::string button_text, unsigned int text_size)
{
	std::string image_path_suffix = "menu_wide.png";
	std::string image_path = "content\\textures_generated\\button_" + image_path_suffix;
	std::string pressed_image_path = "content\\textures_generated\\pressed_button_" + image_path_suffix;
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id].image_path = image_path;
	GetComponent<DrawInfo>()[id].scale_to_fit = true;
	GetComponent<DrawPriority>()[id].draw_priority = 1;
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(width, height) };
	GetComponent<CanReceivePress>()[id] = {};
	GetComponent<KeyConfigButton>()[id].key = key;
	GetComponent<KeyConfigButton>()[id].image_path = image_path;
	GetComponent<KeyConfigButton>()[id].pressed_image_path = pressed_image_path;
	GetComponent<Text>()[id].content = button_text;
	GetComponent<Text>()[id].size = text_size;
	return id;
}

int CreateScreenwideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority)
{
	int id = level.CreateEntityId();
	level.GetComponent<Position>()[id].position = level.size / 2.f;
	level.GetComponent<WidthAndHeight>()[id].width_and_height = level.size;
	level.GetComponent<DrawPriority>()[id].draw_priority = draw_priority;
	level.GetComponent<DrawInfo>()[id].image_path = "content\\textures\\transparent.png";
	level.GetComponent<Shader>()[id].fragment_shader_path = shader_path;
	level.GetComponent<Shader>()[id].float_uniforms["_time"];
	level.GetComponent<Shader>()[id].vec_uniforms["_window_resolution"] = sf::Vector2f(globals.render_window.getSize());
	return id;
}

LevelState ComputeState(Level& level)
{
	for (auto& [entity_id, goal] : level.GetEntitiesWith<Goal>())
	{
		if (goal->is_goal)
		{
			return WON;
		}
	}
	if (level.GetEntitiesWith<Player>().size() == 0)
	{
		return FAILED;
	}
	return PLAYING;
}