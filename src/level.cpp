#include "level.hpp"
#include "game.hpp"
#include "string_parsing_utils.hpp"
#include <functional>

int Level::next_available_entity_id_ = 0;

int Level::CopyEntity(int from_id)
{
	int to_id = CreateEntityId();
	for (auto& [_, component_map_variant] : components_)
	{
		std::visit([from_id, to_id](auto& component_map) {
			if (component_map.count(from_id) != 0)
			{
				component_map[to_id] = component_map[from_id];
			}
		},
			component_map_variant);
	}
	return to_id;
}

void Level::DeleteEntity(int id)
{
	for (auto& [_, component_map_variant] : components_)
	{
		std::visit([id](auto& component_map) {
			if (component_map.count(id) != 0)
			{
				component_map.erase(id);
			}
		},
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

int Level::AddLevelButton(std::function<void(void)> on_click, float pos_x, float pos_y, float width, float height, std::string image_path)
{
	int id = CreateEntityId();
	GetComponent<DrawInfo>()[id].image_path = image_path;
	GetComponent<DrawInfo>()[id].scale_to_fit = true;
	GetComponent<DrawPriority>()[id].draw_priority = 1;
	GetComponent<Position>()[id] = { sf::Vector2f(pos_x, pos_y) };
	GetComponent<WidthAndHeight>()[id] = { sf::Vector2f(width, height) };
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<Border>()[id];
	GetComponent<Button>()[id].on_click = on_click;
	GetComponent<Button>()[id].image_path = image_path;
	return id;
}

int Level::AddMenuButton(std::function<void(void)> on_click, float pos_x, float pos_y, float width, float height, std::string button_text, unsigned int text_size)
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
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<Button>()[id].on_click = on_click;
	GetComponent<Button>()[id].image_path = image_path;
	GetComponent<Button>()[id].pressed_image_path = pressed_image_path;
	GetComponent<Text>()[id].content = button_text;
	GetComponent<Text>()[id].size = text_size;
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
	GetComponent<ClickedOn>()[id] = {};
	GetComponent<KeyConfigButton>()[id].key = key;
	GetComponent<KeyConfigButton>()[id].image_path = image_path;
	GetComponent<KeyConfigButton>()[id].pressed_image_path = pressed_image_path;
	GetComponent<Text>()[id].content = button_text;
	GetComponent<Text>()[id].size = text_size;
	return id;
}