#include "level.hpp"
#include "game.hpp"
#include "string_parsing_utils.hpp"
#include <functional>
#include <iostream>
#include <optional>
#include <string>

int Level::next_available_entity_id_ = 0;
const std::array<sf::Vector2u, 5> LEVEL_SIZES { { sf::Vector2u(16, 9), sf::Vector2u(32, 18), sf::Vector2u(48, 27), sf::Vector2u(64, 36), sf::Vector2u(80, 45) } };

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

void Level::DeleteEntity(std::optional<int> id)
{
	if (id.has_value())
	{
		DeleteEntity(id.value());
	}
}

void Level::DeleteEntity(int id)
{
	if (HasComponents<Children>(id))
	{
		for (auto& [component_type_id, child_ids] : GetComponent<Children>(id)->ids_owned_by_component)
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

LevelState Level::ComputeState()
{
	for (auto& [entity_id, goal] : GetEntitiesWith<Goal>())
	{
		if (goal->is_goal)
		{
			return COMPLETED;
		}
	}
	if (GetEntitiesWith<Player>().size() == 0)
	{
		return FAILED;
	}
	return PLAYING;
}

LevelMode Level::GetMode()
{
	return mode_;
}

void Level::SetMode(LevelMode level_mode)
{
	if (mode_ == EDIT_MODE)
	{
		SaveToFile();
		LoadFromFile();
	}
	if (level_mode == EDIT_MODE)
	{
		LoadFromFile();
	}
	mode_ = level_mode;
}

sf::Vector2f Level::GetSize()
{
	return sf::Vector2f(LEVEL_SIZES[grid_size_id]) * float(BLOCK_SIZE);
}

sf::Vector2u Level::GetGridSize()
{
	return LEVEL_SIZES[grid_size_id];
}

float Level::GetScale()
{
	return (float)LEVEL_SIZES[grid_size_id].x / (float)LEVEL_SIZES[DEFAULT_LEVEL_GRID_SIZE_ID].x;
}

void Level::ResetSize()
{
	grid_size_id = DEFAULT_LEVEL_GRID_SIZE_ID;
}

void Level::IncreaseSize()
{
	if (grid_size_id < LEVEL_SIZES.size() - 1)
	{
		grid_size_id++;
	}
}

void Level::DecreaseSize()
{
	if (grid_size_id > 0)
	{
		grid_size_id--;
	}
}

void Level::LoadFromFile()
{
	LoadFromFile(savefile_path_);
}

void Level::SaveToFile()
{
	SaveToFile(savefile_path_);
}

int AddMenuButton(Level& level, std::function<void(void)> on_click, float pos_x, float pos_y, std::string button_text)
{
	int id = level.AddBlueprint(BPMenuNavigationButton);
	level.GetComponent<Position>(id)->position = sf::Vector2f(pos_x, pos_y);
	level.GetComponent<OnReleasedThisFrame>(id)->func = on_click;
	level.GetComponent<Text>(id)->content = button_text;
	return id;
}
std::vector<int> AddButtonList(Level& level, sf::Vector2f position, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts, std::vector<sf::Keyboard::Key> shortcut_keys, float x_scale, float y_scale, UiOrigin ui_origin)
{
	int n = button_functions.size();
	assert(button_texts.size() == n);

	std::vector<int> ids = {};

	int navigator_id = level.AddBlueprint(BPMenuNavigator);
	ids.push_back(navigator_id);

	for (unsigned i = 0; i < n; ++i)
	{
		int id = level.AddBlueprint(BPMenuNavigationButton);
		ids.push_back(id);
		level.GetComponent<WidthAndHeight>(id)->width_and_height.x *= x_scale;
		float& h = level.GetComponent<WidthAndHeight>(id)->width_and_height.y;
		h *= y_scale;
		level.GetComponent<OnReleasedThisFrame>(id)->func = button_functions[i];
		level.GetComponent<Text>(id)->content = button_texts[i];
		if (shortcut_keys.size() != 0)
		{
			assert(shortcut_keys.size() == n);
			level.AddComponent<ShortcutKey>(id)->key = shortcut_keys[i];
		}
		level.GetComponent<Text>(id)->size *= y_scale;

		float x = 0;
		float spacing = h * 0.5;
		float y = (h + spacing) * i + h / 2;
		float total_list_h = n * h + (n - 1) * spacing;
		switch (ui_origin)
		{
			case TopCenter:
				break;
			case CenterCenter:
				y -= total_list_h / 2;
				break;
			case BottomCenter:
				y -= total_list_h;
				break;
			default:
				assert(false);
		}
		level.GetComponent<Position>(id)->position = position + sf::Vector2f(x, y);
	}

	level.GetComponent<WidthAndHeight>(navigator_id)->width_and_height *= y_scale;
	return ids;
}

std::vector<int> AddOptionsButtonList(Level& level, sf::Vector2f position, std::vector<std::function<void(void)>> button_functions, std::vector<int> button_text_ids)
{
	int n = button_functions.size();
	assert(button_text_ids.size() == n);
	std::vector<int> ids = {};

	for (unsigned i = 0; i < n; ++i)
	{
		int id = level.AddBlueprint("BPButton");
		ids.push_back(id);
		level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
		level.GetComponent<OnReleasedThisFrame>(id)->func = button_functions[i];

		float& h = level.GetComponent<WidthAndHeight>(id)->width_and_height.y;
		float y = (1.5 * h) * i + h / 2;

		level.GetComponent<Position>(id)->position = position + sf::Vector2f(0, y);
	}
	return (ids);
}

std::vector<int> AddKeyConfigButtonList(Level& level, sf::Vector2f position, std::vector<sf::Keyboard::Key*> keys, std::vector<int> button_text_ids)
{
	int n = keys.size();
	assert(button_text_ids.size() == n);
	std::vector<int> ids = {};

	for (unsigned i = 0; i < n; ++i)
	{
		int id = level.AddBlueprint("BPButton");
		ids.push_back(id);
		level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
		level.AddComponent<KeyConfigButton>(id)->key = keys[i];
		level.AddComponent<KeyConfigButton>(id)->button_text = &level.AddComponent<Text>(button_text_ids[i])->content;
		level.AddComponent<StickyButton>(id);
		std::cout << id << std::endl;

		float h = BLOCK_SIZE;
		float y = (3 * h) * i;

		level.GetComponent<Position>(id)->position = position + sf::Vector2f(0, y);
	}
	return (ids);
}

std::vector<int> AddOptionsDescriptionTextList(Level& level, sf::Vector2f position, std::vector<std::string> description_texts)
{
	std::vector<int> ids;

	for (unsigned i = 0; i < description_texts.size(); ++i)
	{
		int id = level.AddBlueprint("BPText");
		ids.push_back(id);
		level.AddComponent<Text>(id)->content = description_texts[i];
		level.AddComponent<Text>(id)->apply_shader = true;

		float h = BLOCK_SIZE;
		float y = (3 * h) * i;

		level.GetComponent<Position>(id)->position = position + sf::Vector2f(0, y);
	}
	return (ids);
}

int AddOptionsButton(Level& level, sf::Keyboard::Key* key, float pos_x, float pos_y, std::string button_text)
{

	int id = level.AddBlueprint(BPButton);
	level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<Position>(id)->position = { sf::Vector2f(pos_x, pos_y) };
	level.AddComponent<KeyConfigButton>(id)->key = key;
	level.AddComponent<Text>(id)->content = button_text;
	level.AddComponent<StickyButton>(id);
	return id;
}

int AddScrollingText(Level& level, float pos_x, float pos_y, std::string text)
{
	int id = level.AddBlueprint(BPText);
	level.AddComponent<Position>(id)->position = { sf::Vector2f(pos_x, pos_y) };
	level.AddComponent<Text>(id)->content = text;
	level.AddComponent<Text>(id)->apply_shader = true;
	return id;
}

int CreateScreenwideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority)
{
	int id = level.CreateEntityId();
	level.AddComponent<Position>(id)->position = level.GetSize() / 2.f;
	level.AddComponent<WidthAndHeight>(id)->width_and_height = level.GetSize();
	level.AddComponent<DrawPriority>(id)->draw_priority = draw_priority;
	level.AddComponent<DrawInfo>(id)->image_path = "content\\textures\\transparent.png";
	level.AddComponent<Shader>(id)->fragment_shader_path = shader_path;
	return id;
}