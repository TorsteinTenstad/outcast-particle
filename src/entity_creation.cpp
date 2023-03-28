#include "entity_creation.hpp"
#include "SFML/Graphics/Text.hpp"
#include "utils/container_operations.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"

entities_handle CreateText(Level& level, sf::Vector2f position, std::string text, unsigned int text_size)
{
	int id = level.AddBlueprint(BPText);
	level.GetComponent<Position>(id)->position = position;
	level.GetComponent<Text>(id)->content = text;
	level.GetComponent<Text>(id)->size = text_size;

	sf::Vector2f width_and_height = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(id)->width_and_height = width_and_height;
	//level.AddComponent<DrawInfo>(id);
	return { std::vector<int> { id }, width_and_height };
}

entities_handle CreateScrollingText(Level& level, sf::Vector2f position, std::string text)
{
	auto [ids, height] = CreateText(level, position, text, 120);
	level.GetComponent<Text>(ids[0])->apply_shader = true;
	return { ids, height };
}

entities_handle CreateButtonTemplate(Level& level, sf::Vector2f position)
{
	int id = level.CreateEntityId();
	level.AddComponent<DrawPriority>(id)->draw_priority = 100;
	level.AddComponent<DrawInfo>(id, { "content\\textures\\white.png", false, 0 });
	level.AddComponent<Shader>(id, { "", "shaders\\round_corners.frag", {}, {}, {} });
	level.AddComponent<FillColor>(id, {});
	level.AddComponent<WidthAndHeight>(id)->width_and_height = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	level.AddComponent<Position>(id)->position = position;
	return { { id }, sf::Vector2f(10, 2) * float(BLOCK_SIZE) };
}

entities_handle CreateMenuButton(Level& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{
	auto [ids, height] = CreateButtonTemplate(level, position);
	int id = ids[0];
	level.AddComponent<Text>(id)->content = button_text;
	level.AddComponent<MouseInteractionDependentFillColor>(id);
	level.AddComponent<ReceivesButtonEvents>(id);
	level.AddComponent<OnReleasedThisFrame>(id)->func = on_click;
	return { ids, height };
}

entities_handle CreateNavigatorButton(Level& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key)
{
	auto [ids, height] = CreateMenuButton(level, position, button_function, button_text);
	level.AddComponent<ShortcutKey>(ids[0])->key = shortcut_key;
	level.AddComponent<MenuNavigatable>(ids[0]);

	return { ids, height };
}

entities_handle CreateKeyConfigButton(Level& level, sf::Vector2f position, sf::Keyboard::Key* key)
{

	auto [ids, height] = CreateButtonTemplate(level, position);
	level.AddComponent<MouseInteractionDependentFillColor>(ids[0]);
	level.AddComponent<ReceivesButtonEvents>(ids[0]);
	level.AddComponent<KeyConfigButton>(ids[0])->key = key;
	level.AddComponent<StickyButton>(ids[0]);
	level.GetComponent<Shader>(ids[0])->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	auto [button_text_id, button_text_height] = CreateScrollingText(level, position, HumanName(*key));
	level.GetComponent<KeyConfigButton>(ids[0])->button_text = &level.GetComponent<Text>(button_text_id[0])->content;
	ids.push_back(button_text_id[0]);

	return { ids, height };
}

entities_handle CreateOptionsButton(Level& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{

	auto [ids, height] = CreateMenuButton(level, position, on_click, "");
	auto [button_text_id, button_text_height] = CreateScrollingText(level, position, button_text);
	level.AddComponent<BinaryOptionsButton>(ids[0])->button_text = &level.GetComponent<Text>(button_text_id[0])->content;
	ids.push_back(button_text_id[0]);
	return { ids, height };
}

//CreateButtonList is in a out-phasing process. Use VerticalEntityLayout for a more flexible variant.
entities_handle CreateButtonList(Level& level, sf::Vector2f position, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts, std::vector<sf::Keyboard::Key> shortcut_keys, float x_scale, float y_scale, UiOrigin ui_origin)
{
	int n = button_functions.size();
	assert(button_texts.size() == n);

	std::vector<int> ids = {};

	int navigator_id = level.AddBlueprint(BPMenuNavigator);
	ids.push_back(navigator_id);

	sf::Vector2f width_and_height;

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
		width_and_height.y = total_list_h;
		width_and_height.x = level.GetComponent<WidthAndHeight>(id)->width_and_height.x;

		level.GetComponent<Position>(id)->position = position + sf::Vector2f(x, y);
	}

	level.GetComponent<WidthAndHeight>(navigator_id)->width_and_height *= y_scale;
	return { ids, width_and_height };
}

entities_handle CreateSliderButton(Level& level, sf::Vector2f position, int* f)
{
	//Add parent button:
	auto [ids, height] = CreateButtonTemplate(level, position);
	int parent_button_id = ids[0];
	level.AddComponent<ReceivesButtonEvents>(parent_button_id);
	level.AddComponent<MouseInteractionDependentFillColor>(parent_button_id, {});
	level.AddComponent<SliderButton>(parent_button_id)->slider_value = f;

	//Creating slider bar:
	auto [slider_bar_ids, slider_bar_height] = CreateButtonTemplate(level, position);
	int slider_bar_id = slider_bar_ids[0];
	level.GetComponent<DrawPriority>(slider_bar_id)->draw_priority = 101;
	level.GetComponent<Position>(slider_bar_id)->position.x -= 1 * BLOCK_SIZE;
	level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height = sf::Vector2f(7, 0.1) * float(BLOCK_SIZE);

	//Creating slider:
	float slider_x_pos = level.GetComponent<Position>(slider_bar_id)->position.x + level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height.x * (*(f)*0.01 - 0.5);

	auto [slider_ids, slider_height] = CreateButtonTemplate(level, sf::Vector2f(slider_x_pos, position.y));
	level.GetComponent<DrawPriority>(slider_ids[0])->draw_priority = 101;
	level.RemoveComponents<WidthAndHeight>(slider_ids[0]);
	level.AddComponent<Radius>(slider_ids[0])->radius = BLOCK_SIZE / 4;

	//Adding text entity
	float text_x_position = level.GetComponent<Position>(slider_bar_id)->position.x + level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height.x / 2 + 1.5 * BLOCK_SIZE;
	auto [button_text_id, button_text_height] = CreateScrollingText(level, sf::Vector2f(text_x_position, position.y), RightShiftString(ToString(*(f)), 3));

	//Connect sliderbar, slider and text to background button
	level.GetComponent<SliderButton>(parent_button_id)->slider_bar_id = slider_bar_id;
	level.GetComponent<SliderButton>(parent_button_id)->slider_button_id = slider_ids[0];
	level.GetComponent<SliderButton>(parent_button_id)->slider_text_id = button_text_id[0];

	ids.insert(ids.begin(), { slider_bar_id, slider_ids[0], button_text_id[0] });
	return { ids, height };
}

entities_handle CreateStatsBadge(Level& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha)
{
	int entity_id = level.CreateEntityId();

	level.AddComponent<DrawInfo>(entity_id, { "content\\textures\\gray.png", false, 0 });
	level.AddComponent<Shader>(entity_id)->fragment_shader_path = "shaders\\stats_badge.frag";
	level.GetComponent<Shader>(entity_id)->int_uniforms["n_collected"] = coin_number;
	level.AddComponent<WidthAndHeight>(entity_id)->width_and_height = sf::Vector2f(7.5, 1.5) * float(BLOCK_SIZE);
	level.AddComponent<Position>(entity_id)->position = position;
	level.AddComponent<FillColor>(entity_id)->color.a = alpha;
	level.AddComponent<Text>(entity_id)->size = 120;
	level.AddComponent<DrawPriority>(entity_id)->draw_priority = 100;

	return { std::vector { entity_id }, level.GetComponent<WidthAndHeight>(entity_id)->width_and_height };
}

int CreateScreenwideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority)
{
	int id = level.CreateEntityId();
	level.AddComponent<Position>(id)->position = level.GetSize() / 2.f;
	level.AddComponent<WidthAndHeight>(id)->width_and_height = level.GetSize();
	level.AddComponent<DrawPriority>(id)->draw_priority = draw_priority;
	level.AddComponent<DrawInfo>(id);
	level.AddComponent<Shader>(id)->fragment_shader_path = shader_path;
	return id;
}

entities_handle VerticalEntityLayout(Level& level, sf::Vector2f position, std::vector<entities_creator> entities_creators, float spacing)
{
	std::function<entities_handle(entities_creator)> f = [](entities_creator entities_creator) { return entities_creator(sf::Vector2f(0, 0)); };
	std::vector<entities_handle> ids_and_heights = ApplyFuncToVector(entities_creators, f);
	sf::Vector2f total_height;
	std::vector<int> total_ids;

	for (auto [ids, height] : ids_and_heights)
	{
		total_height.y += height.y;
		total_height.x = std::max(total_height.x, height.x);
	}
	total_height.y += spacing * (ids_and_heights.size() - 1);

	float current_height = -total_height.y / 2;
	for (auto [ids, height] : ids_and_heights)
	{
		current_height += height.y / 2;
		for (int id : ids)
		{
			level.GetComponent<Position>(id)->position += sf::Vector2f(position.x, current_height + position.y);
			total_ids.push_back(id);
		}
		current_height += height.y / 2 + spacing;
	}
	return (std::tuple { total_ids, total_height });
}
