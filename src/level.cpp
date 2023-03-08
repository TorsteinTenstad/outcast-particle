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

std::vector<int> AddMenuButton(Level& level, std::function<void(void)> on_click, float pos_x, float pos_y, std::string button_text)
{
	int id = level.AddBlueprint(BPMenuNavigationButton);
	level.GetComponent<Position>(id)->position = sf::Vector2f(pos_x, pos_y);
	level.GetComponent<OnReleasedThisFrame>(id)->func = on_click;
	level.GetComponent<Text>(id)->content = button_text;
	std::vector<int> ids { id };
	return ids;
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

std::vector<int> AddKeyConfigButton(Level& level, sf::Keyboard::Key* key, sf::Vector2f button_position)
{

	int id = level.AddBlueprint(BPButton);
	level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<Position>(id)->position = button_position;
	level.AddComponent<KeyConfigButton>(id)->key = key;
	level.AddComponent<StickyButton>(id);
	int button_text_id = AddScrollingText(level, button_position, HumanName(*key));
	level.GetComponent<KeyConfigButton>(id)->button_text = &level.AddComponent<Text>(button_text_id)->content;

	std::vector<int> ids { id, button_text_id };
	return ids;
}

std::vector<int> AddOptionsButton(Level& level, std::function<void(void)> on_click, std::string button_text, sf::Vector2f button_position)
{

	int id = level.AddBlueprint(BPButton);
	level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<Position>(id)->position = button_position;
	level.AddComponent<OnReleasedThisFrame>(id)->func = on_click;

	int button_text_id = AddScrollingText(level, button_position, button_text);
	level.AddComponent<BinaryOptionsButton>(id)->button_text = &level.AddComponent<Text>(button_text_id)->content;
	std::vector<int> ids { id, button_text_id };
	return ids;
}

std::vector<int> AddSliderButton(Level& level, int* f, sf::Vector2f button_position)
{
	//Add parent button:
	int parent_button_id = level.CreateEntityId();
	level.AddComponent<ReceivesButtonEvents>(parent_button_id);
	level.AddComponent<DrawPriority>(parent_button_id)->draw_priority = 100;
	level.AddComponent<DrawInfo>(parent_button_id, { "content\\textures\\white.png", false, 0 });
	level.AddComponent<FillColor>(parent_button_id, {});
	level.AddComponent<MouseInteractionDependentFillColor>(parent_button_id, {});
	level.AddComponent<Shader>(parent_button_id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<WidthAndHeight>(parent_button_id)->width_and_height = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	level.AddComponent<Position>(parent_button_id)->position = sf::Vector2f(button_position.x, button_position.y);
	level.AddComponent<SliderButton>(parent_button_id)->slider_value = f;

	//Creating slider bar:
	int slider_bar_id = level.CreateEntityId();
	level.AddComponent<DrawPriority>(slider_bar_id)->draw_priority = 101;
	level.AddComponent<DrawInfo>(slider_bar_id, { "content\\textures\\white.png", false, 0 });
	level.AddComponent<FillColor>(slider_bar_id, {});
	level.AddComponent<Shader>(slider_bar_id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<Position>(slider_bar_id)->position = button_position;
	level.GetComponent<Position>(slider_bar_id)->position.x -= 1 * BLOCK_SIZE;
	level.AddComponent<WidthAndHeight>(slider_bar_id)->width_and_height = sf::Vector2f(7, 0.1) * float(BLOCK_SIZE);

	//Creating slider:
	float slider_x_pos = level.GetComponent<Position>(slider_bar_id)->position.x + level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height.x * (*(f)*0.01 - 0.5);

	int slider_id = level.CreateEntityId();
	level.AddComponent<DrawPriority>(slider_id)->draw_priority = 101;
	level.AddComponent<DrawInfo>(slider_id, { "content\\textures\\white.png", false, 0 });
	level.AddComponent<FillColor>(slider_id, {});
	level.AddComponent<Shader>(slider_id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<Radius>(slider_id)->radius = BLOCK_SIZE / 4;
	level.AddComponent<Position>(slider_id)->position = sf::Vector2f(slider_x_pos, button_position.y);

	//Adding text entity
	float text_x_position = level.GetComponent<Position>(slider_bar_id)->position.x + level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height.x / 2 + 1.5 * BLOCK_SIZE;
	int button_text_id = AddScrollingText(level, sf::Vector2f(text_x_position, button_position.y), LeftOrRightShiftString(std::vector { ToString(*(f)) }, 3, true)[0]);

	//Connect sliderbar, slider and text to background button
	level.GetComponent<SliderButton>(parent_button_id)->slider_bar_id = slider_bar_id;
	level.GetComponent<SliderButton>(parent_button_id)->slider_x_pos = &level.GetComponent<Position>(slider_id)->position.x;
	level.GetComponent<SliderButton>(parent_button_id)->button_text = &level.GetComponent<Text>(button_text_id)->content;

	std::vector<int> ids { slider_bar_id, slider_id, parent_button_id, button_text_id };
	return ids;
}

int AddScrollingText(Level& level, sf::Vector2f position, std::string text)
{
	int id = level.AddBlueprint(BPText);
	level.AddComponent<Position>(id)->position = position;
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