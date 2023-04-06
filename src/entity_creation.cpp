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

entity_handle CreateText(ECSScene& level, sf::Vector2f position, std::string text, unsigned int text_size)
{
	int id = level.CreateEntityId();
	level.AddComponent<Position>(id)->position = position;
	level.AddComponent<DrawPriority>(id)->draw_priority = UI_BASE_DRAW_PRIORITY + 1;
	level.AddComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll.frag";
	level.AddComponent<Text>(id)->content = text;
	level.GetComponent<Text>(id)->size = text_size;

	sf::Vector2f width_and_height = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(id)->width_and_height = width_and_height;
	//level.AddComponent<DrawInfo>(id); // For debugging layout
	return { id, width_and_height };
}

entity_handle CreateScrollingText(ECSScene& level, sf::Vector2f position, std::string text)
{
	auto [id, size] = CreateText(level, position, text, 120);
	level.GetComponent<Text>(id)->apply_shader = true;
	return { id, size };
}

entity_handle CreateTexturedRectangle(ECSScene& level, sf::Vector2f position, sf::Vector2f size, int draw_priority, std::string image_path, bool scale_to_fit)
{
	int id = level.CreateEntityId();
	level.AddComponent<DrawInfo>(id, { image_path, scale_to_fit, 0 });
	level.AddComponent<DrawPriority>(id)->draw_priority = draw_priority;
	level.AddComponent<WidthAndHeight>(id)->width_and_height = size;
	level.AddComponent<Position>(id)->position = position;
	return { id, size };
}
entity_handle CreateButtonTemplate(ECSScene& level, sf::Vector2f position, sf::Vector2f size)
{
	auto [id, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, "content\\textures\\white.png", false);
	level.AddComponent<FillColor>(id);
	level.AddComponent<Shader>(id, { "", "shaders\\round_corners.frag", {}, {}, {} });
	return { id, size };
}
entity_handle CreateSizedButtonTemplate(ECSScene& level, sf::Vector2f position)
{
	sf::Vector2f size = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	auto [id, _] = CreateButtonTemplate(level, position, size);
	return { id, size };
}
entity_handle CreateButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> on_click, std::string text, unsigned textsize)
{
	auto [id, _] = CreateButtonTemplate(level, position, size);
	level.AddComponent<Text>(id, { text, textsize });
	level.AddComponent<MouseInteractionDependentFillColor>(id);
	level.AddComponent<ReceivesButtonEvents>(id);
	level.AddComponent<OnReleasedThisFrame>(id)->func = on_click;
	return { id, size };
}

entity_handle CreateMenuButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{
	auto [id, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<Text>(id)->content = button_text;
	level.AddComponent<MouseInteractionDependentFillColor>(id);
	level.AddComponent<ReceivesButtonEvents>(id);
	level.AddComponent<OnReleasedThisFrame>(id)->func = on_click;
	return { id, size };
}

entity_handle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key)
{
	auto [id, size] = CreateMenuButton(level, position, button_function, button_text);
	level.AddComponent<ShortcutKey>(id)->key = shortcut_key;
	level.AddComponent<MenuNavigable>(id);

	return { id, size };
}

entity_handle CreateMenuNavigator(ECSScene& level, float buttons_height_in_block_size)
{
	sf::Vector2f size = sf::Vector2f(1, 1.5) * float(BLOCK_SIZE) * buttons_height_in_block_size / 2.f;
	auto [id, _] = CreateTexturedRectangle(level, sf::Vector2f(0, 0), size, UI_BASE_DRAW_PRIORITY + 2, "content\\textures\\menu_navigator.png", true);
	level.AddComponent<FillColor>(id)->color = sf::Color(120, 120, 120);
	level.AddComponent<MenuNavigator>(id);
	return { id, size };
}

entities_handle CreateKeyConfigButton(ECSScene& level, sf::Vector2f position, sf::Keyboard::Key* key)
{
	std::vector<int> ids = {};
	auto [id, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<MouseInteractionDependentFillColor>(id);
	level.AddComponent<ReceivesButtonEvents>(id);
	level.AddComponent<KeyConfigButton>(id)->key = key;
	level.AddComponent<StickyButton>(id);
	level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	auto [button_text_id, button_text_height] = CreateScrollingText(level, position, HumanName(*key));
	level.GetComponent<KeyConfigButton>(id)->button_text = &level.GetComponent<Text>(button_text_id)->content;

	return { { id, button_text_id }, size };
}

entities_handle CreateOptionsButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{

	auto [id, size] = CreateMenuButton(level, position, on_click, "");
	auto [button_text_id, button_text_height] = CreateScrollingText(level, position, button_text);
	level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<BinaryOptionsButton>(id)->button_text = &level.GetComponent<Text>(button_text_id)->content;
	return { { id, button_text_id }, size };
}

entity_handle CreateTimerButton(ECSScene& level, sf::Vector2f position)
{
	auto [id, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponents<Text, TimerButton>(id);
	level.GetComponent<WidthAndHeight>(id)->width_and_height = sf::Vector2f(5 * BLOCK_SIZE, 1 * BLOCK_SIZE);
	level.GetComponent<FillColor>(id)->color.a = 50;
	return { id, size };
}

entities_handle CreateSliderButton(ECSScene& level, sf::Vector2f position, int* f)
{
	//Add parent button:
	auto [parent_button_id, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<ReceivesButtonEvents>(parent_button_id);
	level.AddComponent<MouseInteractionDependentFillColor>(parent_button_id, {});
	level.AddComponent<SliderButton>(parent_button_id)->slider_value = f;

	//Creating slider bar:
	auto [slider_bar_id, slider_bar_height] = CreateSizedButtonTemplate(level, position);
	level.GetComponent<DrawPriority>(slider_bar_id)->draw_priority = 101;
	level.GetComponent<Position>(slider_bar_id)->position.x -= 1 * BLOCK_SIZE;
	level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height = sf::Vector2f(7, 0.1) * float(BLOCK_SIZE);

	//Creating slider:
	float slider_x_pos = level.GetComponent<Position>(slider_bar_id)->position.x + level.GetComponent<WidthAndHeight>(slider_bar_id)->width_and_height.x * (*(f)*0.01 - 0.5);

	auto [slider_id, slider_height] = CreateSizedButtonTemplate(level, sf::Vector2f(slider_x_pos, position.y));
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

entity_handle CreateStatsBadge(ECSScene& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle)
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

// Remove this version of VerticalEntityLayout?
entities_handle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<entities_creator> entities_creators, float spacing)
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

entities_handle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<entities_handle> entities_handles, float spacing)
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