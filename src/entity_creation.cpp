#include "entity_creation.hpp"
#include "SFML/Graphics/Text.hpp"
#include "utils/container_operations.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"

entities_creator AdaptToEntitiesCreator(entity_creator entity_creator)
{
	return [entity_creator](sf::Vector2f position) {
		auto [id, size] = entity_creator(position);
		return entities_handle({ id }, size);
	};
}

entities_handle AdaptToEntitiesHandle(entity_handle entity_handle)
{
	auto [id, size] = entity_handle;
	return entities_handle({ id }, size);
}

entity_handle CreateText(Level& level, sf::Vector2f position, std::string text, unsigned int text_size)
{
	int id = level.AddBlueprint(BPText);
	level.GetComponent<Position>(id)->position = position;
	level.GetComponent<Text>(id)->content = text;
	level.GetComponent<Text>(id)->size = text_size;

	sf::Vector2f width_and_height = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(id)->width_and_height = width_and_height;
	//level.AddComponent<DrawInfo>(id);
	return { id, width_and_height };
}

entity_handle CreateScrollingText(Level& level, sf::Vector2f position, std::string text)
{
	auto [id, size] = CreateText(level, position, text, 120);
	level.GetComponent<Text>(id)->apply_shader = true;
	return { id, size };
}

entity_handle CreateButtonTemplate(ECSScene& level, sf::Vector2f position)
{
	int id = level.CreateEntityId();
	level.AddComponent<DrawPriority>(id)->draw_priority = 100;
	level.AddComponent<DrawInfo>(id, { "content\\textures\\white.png", false, 0 });
	level.AddComponent<Shader>(id, { "", "shaders\\round_corners.frag", {}, {}, {} });
	level.AddComponent<FillColor>(id, {});
	sf::Vector2f width_and_height = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(id)->width_and_height = width_and_height;
	level.AddComponent<Position>(id)->position = position;
	return { id, width_and_height };
}

entity_handle CreateMenuButton(Level& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{
	auto [id, size] = CreateButtonTemplate(level, position);
	level.AddComponent<Text>(id)->content = button_text;
	level.AddComponent<MouseInteractionDependentFillColor>(id);
	level.AddComponent<ReceivesButtonEvents>(id);
	level.AddComponent<OnReleasedThisFrame>(id)->func = on_click;
	return { id, size };
}

entity_handle CreateNavigatorButton(Level& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key)
{
	auto [id, size] = CreateMenuButton(level, position, button_function, button_text);
	level.AddComponent<ShortcutKey>(id)->key = shortcut_key;
	level.AddComponent<MenuNavigable>(id);

	return { id, size };
}

entities_handle CreateKeyConfigButton(Level& level, sf::Vector2f position, sf::Keyboard::Key* key)
{
	std::vector<int> ids = {};
	auto [id, size] = CreateButtonTemplate(level, position);
	level.AddComponent<MouseInteractionDependentFillColor>(id);
	level.AddComponent<ReceivesButtonEvents>(id);
	level.AddComponent<KeyConfigButton>(id)->key = key;
	level.AddComponent<StickyButton>(id);
	level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	auto [button_text_id, button_text_height] = CreateScrollingText(level, position, HumanName(*key));
	level.GetComponent<KeyConfigButton>(id)->button_text = &level.GetComponent<Text>(button_text_id)->content;

	return { { id, button_text_id }, size };
}

entities_handle CreateOptionsButton(Level& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{

	auto [id, size] = CreateMenuButton(level, position, on_click, "");
	auto [button_text_id, button_text_height] = CreateScrollingText(level, position, button_text);
	level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<BinaryOptionsButton>(id)->button_text = &level.GetComponent<Text>(button_text_id)->content;
	return { { id, button_text_id }, size };
}

entity_handle CreateTimerButton(ECSScene& level, sf::Vector2f position)
{
	auto [id, size] = CreateButtonTemplate(level, position);
	level.AddComponents<Text, TimerButton>(id);
	level.GetComponent<WidthAndHeight>(id)->width_and_height = sf::Vector2f(5 * BLOCK_SIZE, 1 * BLOCK_SIZE);
	level.GetComponent<FillColor>(id)->color.a = 50;
	return { id, size };
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
	auto [parent_button_id, size] = CreateButtonTemplate(level, position);
	level.AddComponent<ReceivesButtonEvents>(parent_button_id);
	level.AddComponent<MouseInteractionDependentFillColor>(parent_button_id, {});
	level.AddComponent<SliderButton>(parent_button_id)->slider_value = f;

	//Creating slider bar:
	auto [slider_bar_id, slider_bar_height] = CreateButtonTemplate(level, position);
	level.GetComponent<DrawPriority>(slider_bar_id)->draw_priority = 101;
	level.GetComponent<Position>(slider_bar_id)->position.x -= 1 * BLOCK_SIZE;
	level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height = sf::Vector2f(7, 0.1) * float(BLOCK_SIZE);

	//Creating slider:
	float slider_x_pos = level.GetComponent<Position>(slider_bar_id)->position.x + level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height.x * (*(f)*0.01 - 0.5);

	auto [slider_id, slider_height] = CreateButtonTemplate(level, sf::Vector2f(slider_x_pos, position.y));
	level.GetComponent<DrawPriority>(slider_id)->draw_priority = 101;
	level.RemoveComponents<WidthAndHeight>(slider_id);
	level.AddComponent<Radius>(slider_id)->radius = BLOCK_SIZE / 4;

	//Adding text entity
	float text_x_position = level.GetComponent<Position>(slider_bar_id)->position.x + level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height.x / 2 + 1.5 * BLOCK_SIZE;
	auto [button_text_id, button_text_height] = CreateScrollingText(level, sf::Vector2f(text_x_position, position.y), RightShiftString(ToString(*(f)), 3));

	//Connect sliderbar, slider and text to background button
	level.GetComponent<SliderButton>(parent_button_id)->slider_bar_id = slider_bar_id;
	level.GetComponent<SliderButton>(parent_button_id)->slider_button_id = slider_id;
	level.GetComponent<SliderButton>(parent_button_id)->slider_text_id = button_text_id;

	return { { parent_button_id, slider_bar_id, slider_id, button_text_id }, size };
}

entity_handle CreateStatsBadge(Level& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle)
{
	int entity_id = level.CreateEntityId();

	level.AddComponent<DrawInfo>(entity_id, { "content\\textures\\gray.png", false, 0 });
	level.AddComponent<Shader>(entity_id)->fragment_shader_path = "shaders\\stats_badge.frag";
	level.GetComponent<Shader>(entity_id)->int_uniforms["n_collected"] = coin_number;
	sf::Vector2f width_and_height = sf::Vector2f(7.5, 1.5) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(entity_id)->width_and_height = width_and_height;
	level.AddComponent<Position>(entity_id)->position = position;
	level.AddComponent<FillColor>(entity_id)->color.a = alpha;
	level.AddComponent<Text>(entity_id)->size = 120;
	level.GetComponent<Text>(entity_id)->content = text;
	level.AddComponent<DrawPriority>(entity_id)->draw_priority = 100;
	if (twinkle)
	{
		level.AddComponent<TwinkleEffect>(entity_id);
		level.AddComponent<Children>(entity_id);
	}

	return { entity_id, width_and_height };
}

int CreateScreenWideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority)
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

	for (auto [ids, size] : ids_and_heights)
	{
		total_height.y += size.y;
		total_height.x = std::max(total_height.x, size.x);
	}
	total_height.y += spacing * (ids_and_heights.size() - 1);

	float current_height = -total_height.y / 2;
	for (auto [ids, size] : ids_and_heights)
	{
		current_height += size.y / 2;
		for (int id : ids)
		{
			level.GetComponent<Position>(id)->position += sf::Vector2f(position.x, current_height + position.y);
			total_ids.push_back(id);
		}
		current_height += size.y / 2 + spacing;
	}
	return (std::tuple { total_ids, total_height });
}

entities_handle VerticalEntityLayout(Level& level, sf::Vector2f position, std::vector<entities_handle> entities_handles, float spacing)
{
	sf::Vector2f total_size;
	std::vector<int> total_ids;

	for (auto [ids, size] : entities_handles)
	{
		total_size.y += size.y;
		total_size.x = std::max(total_size.x, size.x);
	}
	total_size.y += spacing * (entities_handles.size() - 1);

	float current_height = -total_size.y / 2;
	for (auto [ids, size] : entities_handles)
	{
		current_height += size.y / 2;
		for (int id : ids)
		{
			level.GetComponent<Position>(id)->position += sf::Vector2f(position.x, current_height + position.y);
			total_ids.push_back(id);
		}
		current_height += size.y / 2 + spacing;
	}
	return (std::tuple { total_ids, total_size });
}