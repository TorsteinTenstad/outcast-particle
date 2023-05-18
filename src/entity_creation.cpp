#include "entity_creation.hpp"
#include "Components/sound_info.hpp"
#include "SFML/Graphics/Text.hpp"
#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/menu_navigator.hpp"
#include "components/position.hpp"
#include "components/scheduled_delete.hpp"
#include "components/screen_wide_shader_effects.hpp"
#include "components/shader.hpp"
#include "components/size.hpp"
#include "components/sticky_button.hpp"
#include "components/text.hpp"
#include "utils/container_operations.hpp"
#include "utils/math.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"

class ConfirmMenuEntity
{};

EntitiesCreator AdaptToEntitiesCreator(EntityCreator EntityCreator)
{
	return [EntityCreator](sf::Vector2f position) {
		auto [entity, size] = EntityCreator(position);
		return EntitiesHandle({ entity }, size);
	};
}

EntitiesHandle ToEntitiesHandle(EntityHandle EntityHandle)
{
	auto [entity, size] = EntityHandle;
	return EntitiesHandle({ entity }, size);
}

EntityHandle CreateText(ECSScene& level, sf::Vector2f position, std::string content, unsigned int text_size)
{
	Entity entity = level.CreateEntity();
	level.AddComponent<Position>(entity)->position = position;
	level.AddComponent<DrawPriority>(entity)->draw_priority = UI_BASE_DRAW_PRIORITY + 1;
	level.AddComponent<Shader>(entity)->fragment_shader_path = "shaders\\scroll.frag";
	level.AddComponent<Text>(entity)->content = content;
	level.GetComponent<Text>(entity)->size = text_size;

	sf::Vector2f width_and_height = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(entity)->width_and_height = width_and_height;
	//level.AddComponent<DrawInfo>(entity); // For debugging layout
	return { entity, width_and_height };
}

EntityHandle CreateScrollingText(ECSScene& level, sf::Vector2f position, std::string content, unsigned int text_size)
{
	auto [entity, size] = CreateText(level, position, content, text_size);
	level.GetComponent<Text>(entity)->apply_shader = true;
	return { entity, size };
}

EntityHandle CreateTexturedRectangle(ECSScene& level, sf::Vector2f position, sf::Vector2f size, int draw_priority, std::string image_path, bool tile)
{
	Entity entity = level.CreateEntity();
	level.AddComponent<DrawInfo>(entity, { image_path, tile, 0 });
	level.AddComponent<DrawPriority>(entity)->draw_priority = draw_priority;
	level.AddComponent<WidthAndHeight>(entity)->width_and_height = size;
	level.AddComponent<Position>(entity)->position = position;
	return { entity, size };
}

EntityHandle CreateButtonTemplate(ECSScene& level, sf::Vector2f position, sf::Vector2f size)
{
	auto [entity, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, "content\\textures\\white.png", false);
	level.AddComponent<FillColor>(entity);
	level.AddComponent<Shader>(entity, { "", "shaders\\round_corners.frag", {}, {}, {} });
	level.AddComponent<SoundInfo>(entity)->sound_paths = { { ON_CLICK, "content\\sounds\\click.wav" } };
	return { entity, size };
}
EntityHandle CreateMouseEventButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size)
{
	auto [entity, _] = CreateButtonTemplate(level, position, size);
	level.AddComponent<MouseInteractionDependentFillColor>(entity);
	level.AddComponent<ReceivesButtonEvents>(entity);
	return { entity, size };
}

EntityHandle CreateSizedButtonTemplate(ECSScene& level, sf::Vector2f position)
{
	sf::Vector2f size = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	auto [entity, _] = CreateButtonTemplate(level, position, size);
	return { entity, size };
}

EntityHandle CreateButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> on_click, std::string text, unsigned textsize)
{
	auto [entity, _] = CreateMouseEventButton(level, position, size);
	level.AddComponent<Text>(entity, { text, textsize });
	level.AddComponent<OnReleasedThisFrame>(entity)->func = on_click;
	return { entity, size };
}

EntityHandle CreateMenuButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{
	auto [entity, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<Text>(entity)->content = button_text;
	level.AddComponent<MouseInteractionDependentFillColor>(entity);
	level.AddComponent<ReceivesButtonEvents>(entity);
	level.AddComponent<OnReleasedThisFrame>(entity)->func = on_click;
	return { entity, size };
}

EntityHandle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key)
{
	auto [entity, size] = CreateMenuButton(level, position, button_function, button_text);
	level.AddComponent<ShortcutKey>(entity)->key = shortcut_key;
	level.AddComponent<MenuNavigable>(entity);

	return { entity, size };
}

EntityHandle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key, sf::Vector2f size)
{
	auto [entity, _] = CreateMenuButton(level, position, button_function, button_text);
	level.AddComponent<ShortcutKey>(entity)->key = shortcut_key;
	level.AddComponent<MenuNavigable>(entity);
	level.GetComponent<WidthAndHeight>(entity)->width_and_height = size;

	return { entity, size };
}

EntityHandle CreateMenuNavigator(ECSScene& level, float buttons_height_in_block_size)
{
	sf::Vector2f size = sf::Vector2f(1, 1.5) * float(BLOCK_SIZE) * buttons_height_in_block_size / 2.f;
	auto [entity, _] = CreateTexturedRectangle(level, sf::Vector2f(0, 0), size, UI_BASE_DRAW_PRIORITY + 2, "content\\textures\\menu_navigator.png", false);
	level.AddComponent<FillColor>(entity)->color = sf::Color(120, 120, 120);
	level.AddComponent<MenuNavigator>(entity);
	level.AddComponent<Shader>(entity)->fragment_shader_path = "shaders\\scroll.frag";
	return { entity, size };
}

EntitiesHandle CreateKeyConfigButton(ECSScene& level, sf::Vector2f position, sf::Keyboard::Key* key)
{
	std::vector<Entity> entities = {};
	auto [entity, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<MouseInteractionDependentFillColor>(entity);
	level.AddComponent<ReceivesButtonEvents>(entity);
	level.AddComponent<KeyConfigButton>(entity)->key = key;
	level.AddComponent<StickyButton>(entity);
	level.GetComponent<Shader>(entity)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	auto [button_text, button_text_size] = CreateScrollingText(level, position, HumanName(*key));
	level.GetComponent<KeyConfigButton>(entity)->button_text = &level.GetComponent<Text>(button_text)->content;

	return { { entity, button_text }, size };
}

EntitiesHandle CreateOptionsButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{

	auto [entity, size] = CreateMenuButton(level, position, on_click, "");
	auto [button_text_entity, button_text_size] = CreateScrollingText(level, position, button_text);
	level.GetComponent<Shader>(entity)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<BinaryOptionsButton>(entity)->button_text = &level.GetComponent<Text>(button_text_entity)->content;
	return { { entity, button_text_entity }, size };
}

EntityHandle CreateTimerButton(ECSScene& level, sf::Vector2f position)
{
	auto [entity, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<Text>(entity);
	level.GetComponent<WidthAndHeight>(entity)->width_and_height = sf::Vector2f(5 * BLOCK_SIZE, 1 * BLOCK_SIZE);
	level.GetComponent<FillColor>(entity)->color.a = 50;
	return { entity, size };
}

EntityHandle CreateScreenWideBlur(ECSScene& level, sf::Vector2f level_size, int draw_priority)
{
	EntityHandle handle = CreateTexturedRectangle(level, level_size / 2.f, level_size, draw_priority, "", false);
	Entity entity = GetEntity(handle);
	level.AddComponent<FillColor>(entity)->color = sf::Color(30, 30, 30, 240);
	level.AddComponent<ReceivesButtonEvents>(entity);
	return handle;
}

EntitiesHandle CreateConfirmMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::function<void(void)> confirm_function)
{
	return CreateBlockingPopupMenu(level, level_size, title, { { "Confirm", confirm_function }, { "Cancel", []() {} } }, {});
}
EntitiesHandle CreateBlockingPopupMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::vector<std::pair<std::string, std::function<void(void)>>> button_functions, EntitiesHandle middle_entities)
{
	auto add_delete_identifier = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponent<ConfirmMenuEntity>(entity); });
	for (Entity entity : GetEntities(middle_entities))
	{
		level.AddComponent<ConfirmMenuEntity>(entity);
	}

	CreateScreenWideBlur(level, level_size, 150);

	EntitiesHandle title_handle = ToEntitiesHandle(CreateText(level, sf::Vector2f(0, 0), title, 150));
	std::vector<EntitiesHandle> button_row;
	auto close_menu = [&level]() {
		for (Entity entity : level.GetEntitiesWithComponent<ConfirmMenuEntity>())
		{
			level.EnsureExistenceOfComponent<ScheduledDelete>(entity)->frames_left_to_live = 0;
		}
	};
	for (const auto& [button_text, button_function] : button_functions)
	{
		auto button_function_with_close = [close_menu = close_menu, button_function = button_function]() {close_menu(); button_function(); };
		button_row.push_back(ToEntitiesHandle(CreateNavigatorButton(level, sf::Vector2f(0, 0), button_function_with_close, button_text, sf::Keyboard::Unknown, sf::Vector2f(6, 2) * float(BLOCK_SIZE))));
	}

	EntitiesHandle button_row_handle = HorizontalEntityLayout(level, sf::Vector2f(0, 0), button_row, level_size.x / 12.f);
	Entity menu_navigator_entity = GetEntity(CreateMenuNavigator(level));
	MenuNavigator* menu_navigator = level.GetComponent<MenuNavigator>(menu_navigator_entity);
	menu_navigator->menu_items = GetEntities(button_row_handle);
	menu_navigator->increment_key = sf::Keyboard::Right;
	menu_navigator->decrement_key = sf::Keyboard::Left;
	level.GetComponent<DrawPriority>(menu_navigator_entity)->draw_priority = 200;
	EntitiesHandle menu_items = VerticalEntityLayout(level, level_size / 2.f, { title_handle, middle_entities, button_row_handle }, level_size.y / 12.f);
	for (Entity entity : GetEntities(menu_items))
	{
		level.GetComponent<DrawPriority>(entity)->draw_priority = 200;
	}
	return menu_items;
}

EntitiesHandle CreateSliderButton(ECSScene& level, sf::Vector2f position, int* f)
{
	//Add parent button:
	auto [parent_button, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<ReceivesButtonEvents>(parent_button);
	level.AddComponent<MouseInteractionDependentFillColor>(parent_button, {});
	level.AddComponent<SliderButton>(parent_button)->slider_value = f;

	//Creating slider bar:
	auto [slider_bar, slider_bar_height] = CreateSizedButtonTemplate(level, position);
	level.GetComponent<DrawPriority>(slider_bar)->draw_priority = 101;
	level.GetComponent<Position>(slider_bar)->position.x -= 1 * BLOCK_SIZE;
	level.GetComponent<WidthAndHeight>(slider_bar)->width_and_height = sf::Vector2f(7, 0.1) * float(BLOCK_SIZE);

	//Creating slider:
	float slider_x_pos = level.GetComponent<Position>(slider_bar)->position.x + level.GetComponent<WidthAndHeight>(slider_bar)->width_and_height.x * (*(f)*0.01 - 0.5);

	auto [slider, slider_height] = CreateSizedButtonTemplate(level, sf::Vector2f(slider_x_pos, position.y));
	level.GetComponent<DrawPriority>(slider)->draw_priority = 101;
	level.RemoveComponents<WidthAndHeight>(slider);
	level.AddComponent<Radius>(slider)->radius = BLOCK_SIZE / 4;

	//Adding text entity
	float text_x_position = level.GetComponent<Position>(slider_bar)->position.x + level.GetComponent<WidthAndHeight>(slider_bar)->width_and_height.x / 2 + 1.5 * BLOCK_SIZE;
	auto [button_text, button_text_size] = CreateScrollingText(level, sf::Vector2f(text_x_position, position.y), RightShiftString(ToString(*(f)), 3));

	//Connect slider bar, slider and text to background button
	level.GetComponent<SliderButton>(parent_button)->slider_bar = slider_bar;
	level.GetComponent<SliderButton>(parent_button)->slider_button = slider;
	level.GetComponent<SliderButton>(parent_button)->slider_text = button_text;

	return { { parent_button, slider_bar, slider, button_text }, size };
}

EntityHandle CreateStatsBadge(ECSScene& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle)
{
	Entity entity = level.CreateEntity();

	level.AddComponent<DrawInfo>(entity, { "content\\textures\\gray.png", false, 0 });
	level.AddComponent<Shader>(entity)->fragment_shader_path = "shaders\\stats_badge.frag";
	level.GetComponent<Shader>(entity)->int_uniforms["n_collected"] = coin_number;
	sf::Vector2f width_and_height = sf::Vector2f(7.5, 1.5) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(entity)->width_and_height = width_and_height;
	level.AddComponent<Position>(entity)->position = position;
	level.AddComponent<FillColor>(entity)->color.a = alpha;
	level.AddComponent<Text>(entity)->size = 100;
	level.GetComponent<Text>(entity)->content = text;
	level.AddComponent<DrawPriority>(entity)->draw_priority = 100;
	if (twinkle)
	{
		level.AddComponent<TwinkleEffect>(entity);
		level.AddComponent<Children>(entity);
	}

	return { entity, width_and_height };
}

Entity CreateScreenWideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority)
{
	Entity entity = level.CreateEntity();
	level.AddComponent<Position>(entity)->position = level.GetSize() / 2.f;
	level.AddComponent<WidthAndHeight>(entity)->width_and_height = level.GetSize();
	level.AddComponent<DrawPriority>(entity)->draw_priority = draw_priority;
	level.AddComponent<DrawInfo>(entity);
	level.AddComponent<Shader>(entity)->fragment_shader_path = shader_path;
	return entity;
}

// Remove this version of VerticalEntityLayout?
EntitiesHandle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesCreator> entities_creators, float spacing)
{
	std::function<EntitiesHandle(EntitiesCreator)> f = [](EntitiesCreator EntitiesCreator) { return EntitiesCreator(sf::Vector2f(0, 0)); };
	std::vector<EntitiesHandle> entities_and_heights = ApplyFuncToVector(entities_creators, f);
	sf::Vector2f total_height;
	std::vector<Entity> total_entities;

	for (auto [entities, size] : entities_and_heights)
	{
		total_height.y += size.y;
		total_height.x = std::max(total_height.x, size.x);
	}
	total_height.y += spacing * (entities_and_heights.size() - 1);

	float current_height = -total_height.y / 2;
	for (auto [entities, size] : entities_and_heights)
	{
		current_height += size.y / 2;
		for (Entity entity : entities)
		{
			level.GetComponent<Position>(entity)->position += sf::Vector2f(position.x, current_height + position.y);
			total_entities.push_back(entity);
		}
		current_height += size.y / 2 + spacing;
	}
	return (std::tuple { total_entities, total_height });
}

static EntitiesHandle OneDimensionalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_handles, float spacing, std::function<float&(sf::Vector2f&)> access_layout_direction, std::function<float&(sf::Vector2f&)> access_other_direction, UiOrigin ui_origin)
{
	std::vector<Entity> total_entities;
	sf::Vector2f total_size = sf::Vector2f(0, 0);

	float& total_size_in_layout_direction = access_layout_direction(total_size);
	float& total_size_in_other_direction = access_other_direction(total_size);

	for (auto [entities, size] : entities_handles)
	{
		if (entities.size() == 0) { continue; }
		total_size_in_layout_direction += access_layout_direction(size);
		total_size_in_other_direction = std::max(total_size_in_other_direction, access_other_direction(size));
		total_size_in_layout_direction += spacing;
	}
	total_size_in_layout_direction -= spacing;

	float current_position_in_layout_direction = 0;
	switch (ui_origin)
	{
		case StartEdge:
			break;
		case Center:
			current_position_in_layout_direction -= total_size_in_layout_direction / 2;
			break;
		case EndEdge:
			current_position_in_layout_direction -= total_size_in_layout_direction;
			break;
		default:
			assert(false);
	}
	for (auto [entities, size] : entities_handles)
	{
		if (entities.size() == 0) { continue; }
		current_position_in_layout_direction += access_layout_direction(size) / 2;
		for (Entity entity : entities)
		{
			sf::Vector2f& entity_position = level.GetComponent<Position>(entity)->position;
			entity_position += position;
			access_layout_direction(entity_position) += current_position_in_layout_direction;
			total_entities.push_back(entity);
		}
		current_position_in_layout_direction += access_layout_direction(size) / 2 + spacing;
	}
	return (std::tuple { total_entities, total_size });
}

EntitiesHandle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_handles, float spacing, UiOrigin ui_origin)
{
	return OneDimensionalEntityLayout(level, position, entities_handles, spacing, GetY, GetX, ui_origin);
}

EntitiesHandle HorizontalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_handles, float spacing, UiOrigin ui_origin)
{
	return OneDimensionalEntityLayout(level, position, entities_handles, spacing, GetX, GetY, ui_origin);
}