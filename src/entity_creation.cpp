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
#include "components/scroll.hpp"
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
		auto [id, size] = EntityCreator(position);
		return EntitiesHandle({ id }, size);
	};
}

EntitiesHandle ToEntitiesHandle(EntityHandle EntityHandle)
{
	auto [id, size] = EntityHandle;
	return EntitiesHandle({ id }, size);
}

EntityHandle CreateScrollWindow(ECSScene& level, sf::Vector2f position, sf::Vector2f width_and_height, float entity_height)
{
	int id = level.CreateEntityId();
	level.AddComponent<Position>(id)->position = position;
	level.AddComponent<WidthAndHeight>(id)->width_and_height = width_and_height;
	level.AddComponent<ScrollWindow>(id)->entity_height = entity_height;
	return { id, width_and_height };
}

EntityHandle CreateText(ECSScene& level, sf::Vector2f position, std::string content, unsigned int text_size)
{
	int id = level.CreateEntityId();
	level.AddComponent<Position>(id)->position = position;
	level.AddComponent<DrawPriority>(id)->draw_priority = UI_BASE_DRAW_PRIORITY + 1;
	level.AddComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll.frag";
	level.AddComponent<Text>(id)->content = content;
	level.GetComponent<Text>(id)->size = text_size;

	sf::Vector2f width_and_height = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(id)->width_and_height = width_and_height;
	//level.AddComponent<DrawInfo>(id); // For debugging layout
	return { id, width_and_height };
}

EntityHandle CreateScrollingText(ECSScene& level, sf::Vector2f position, std::string content, unsigned int text_size)
{
	auto [id, size] = CreateText(level, position, content, text_size);
	level.GetComponent<Text>(id)->apply_shader = true;
	return { id, size };
}

EntityHandle CreateTexturedRectangle(ECSScene& level, sf::Vector2f position, sf::Vector2f size, int draw_priority, std::string image_path, bool tile)
{
	int id = level.CreateEntityId();
	level.AddComponent<DrawInfo>(id, { image_path, tile, 0 });
	level.AddComponent<DrawPriority>(id)->draw_priority = draw_priority;
	level.AddComponent<WidthAndHeight>(id)->width_and_height = size;
	level.AddComponent<Position>(id)->position = position;
	return { id, size };
}

EntityHandle CreateButtonTemplate(ECSScene& level, sf::Vector2f position, sf::Vector2f size)
{
	auto [id, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, "content\\textures\\white.png", false);
	level.AddComponent<FillColor>(id);
	level.AddComponent<Shader>(id, { "", "shaders\\round_corners.frag", {}, {}, {} });
	level.AddComponent<SoundInfo>(id)->sound_paths = { { ON_CLICK, "content\\sounds\\click.wav" } };
	return { id, size };
}
EntityHandle CreateMouseEventButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size)
{
	auto [id, _] = CreateButtonTemplate(level, position, size);
	level.AddComponent<MouseInteractionDependentFillColor>(id);
	level.AddComponent<ReceivesButtonEvents>(id);
	return { id, size };
}
EntityHandle CreateSizedButtonTemplate(ECSScene& level, sf::Vector2f position)
{
	sf::Vector2f size = sf::Vector2f(10, 2) * float(BLOCK_SIZE);
	auto [id, _] = CreateButtonTemplate(level, position, size);
	return { id, size };
}
EntityHandle CreateButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> on_click, std::string text, unsigned textsize)
{
	auto [id, _] = CreateMouseEventButton(level, position, size);
	level.AddComponent<Text>(id, { text, textsize });
	level.AddComponent<OnReleasedThisFrame>(id)->func = on_click;
	return { id, size };
}

EntityHandle CreateMenuButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{
	auto [id, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<Text>(id)->content = button_text;
	level.AddComponent<MouseInteractionDependentFillColor>(id);
	level.AddComponent<ReceivesButtonEvents>(id);
	level.AddComponent<OnReleasedThisFrame>(id)->func = on_click;
	return { id, size };
}

EntityHandle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key)
{
	auto [id, size] = CreateMenuButton(level, position, button_function, button_text);
	level.AddComponent<ShortcutKey>(id)->key = shortcut_key;
	level.AddComponent<MenuNavigable>(id);

	return { id, size };
}

EntityHandle CreateMenuNavigator(ECSScene& level, float buttons_height_in_block_size)
{
	sf::Vector2f size = sf::Vector2f(1, 1.5) * float(BLOCK_SIZE) * buttons_height_in_block_size / 2.f;
	auto [id, _] = CreateTexturedRectangle(level, sf::Vector2f(0, 0), size, UI_BASE_DRAW_PRIORITY + 2, "content\\textures\\menu_navigator.png", false);
	level.AddComponent<FillColor>(id)->color = sf::Color(120, 120, 120);
	level.AddComponent<MenuNavigator>(id);
	level.AddComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll.frag";
	return { id, size };
}

EntitiesHandle CreateKeyConfigButton(ECSScene& level, sf::Vector2f position, sf::Keyboard::Key* key)
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

EntitiesHandle CreateOptionsButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text)
{

	auto [id, size] = CreateMenuButton(level, position, on_click, "");
	auto [button_text_id, button_text_height] = CreateScrollingText(level, position, button_text);
	level.GetComponent<Shader>(id)->fragment_shader_path = "shaders\\scroll_and_round_corners.frag";
	level.AddComponent<BinaryOptionsButton>(id)->button_text = &level.GetComponent<Text>(button_text_id)->content;
	return { { id, button_text_id }, size };
}

EntityHandle CreateTimerButton(ECSScene& level, sf::Vector2f position)
{
	auto [id, size] = CreateSizedButtonTemplate(level, position);
	level.AddComponent<Text>(id);
	level.GetComponent<WidthAndHeight>(id)->width_and_height = sf::Vector2f(5 * BLOCK_SIZE, 1 * BLOCK_SIZE);
	level.GetComponent<FillColor>(id)->color.a = 50;
	return { id, size };
}

EntityHandle CreateScreenWideBlur(ECSScene& level, sf::Vector2f level_size, int draw_priority)
{
	EntityHandle handle = CreateTexturedRectangle(level, level_size / 2.f, level_size, draw_priority, "", false);
	int id = GetId(handle);
	level.AddComponent<FillColor>(id)->color = sf::Color(30, 30, 30, 220);
	level.AddComponent<ReceivesButtonEvents>(id);
	return handle;
}

EntitiesHandle CreateConfirmMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::function<void(void)> confirm_function)
{
	return CreateBlockingPopupMenu(level, level_size, title, { { "Confirm", confirm_function }, { "Cancel", []() {} } }, {});
}
EntitiesHandle CreateBlockingPopupMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::vector<std::pair<std::string, std::function<void(void)>>> button_functions, EntitiesHandle middle_entities)
{
	auto add_delete_identifier = EntityCreationObserver(level, [](ECSScene& level, int id) { level.AddComponent<ConfirmMenuEntity>(id); });
	for (int id : GetIds(middle_entities))
	{
		level.AddComponent<ConfirmMenuEntity>(id);
	}

	CreateScreenWideBlur(level, level_size, 150);

	EntitiesHandle title_handle = ToEntitiesHandle(CreateText(level, sf::Vector2f(0, 0), title, 150));
	std::vector<EntitiesHandle> button_row;
	auto close_menu = [&level]() {
		for (int entity_id : level.GetIdsWithComponent<ConfirmMenuEntity>())
		{
			level.EnsureExistenceOfComponent<ScheduledDelete>(entity_id)->frames_left_to_live = 0;
		}
	};
	for (const auto& [button_text, button_function] : button_functions)
	{
		auto button_function_with_close = [close_menu = close_menu, button_function = button_function]() {close_menu(); button_function(); };
		button_row.push_back(ToEntitiesHandle(CreateNavigatorButton(level, sf::Vector2f(0, 0), button_function_with_close, button_text, sf::Keyboard::Unknown)));
	}

	EntitiesHandle button_row_handle = HorizontalEntityLayout(level, sf::Vector2f(0, 0), button_row, level_size.x / 12.f);
	int menu_navigator_id = GetId(CreateMenuNavigator(level));
	MenuNavigator* menu_navigator = level.GetComponent<MenuNavigator>(menu_navigator_id);
	menu_navigator->menu_items = GetIds(button_row_handle);
	menu_navigator->increment_key = sf::Keyboard::Left;
	menu_navigator->decrement_key = sf::Keyboard::Right;
	level.GetComponent<DrawPriority>(menu_navigator_id)->draw_priority = 200;
	EntitiesHandle menu_items = VerticalEntityLayout(level, level_size / 2.f, { title_handle, middle_entities, button_row_handle }, level_size.y / 6.f);
	for (int id : GetIds(menu_items))
	{
		level.GetComponent<DrawPriority>(id)->draw_priority = 200;
	}
	return menu_items;
}

EntitiesHandle CreateSliderButton(ECSScene& level, sf::Vector2f position, int* f)
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

EntityHandle CreateStatsBadge(ECSScene& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle)
{
	int entity_id = level.CreateEntityId();

	level.AddComponent<DrawInfo>(entity_id, { "content\\textures\\gray.png", false, 0 });
	level.AddComponent<Shader>(entity_id)->fragment_shader_path = "shaders\\stats_badge.frag";
	level.GetComponent<Shader>(entity_id)->int_uniforms["n_collected"] = coin_number;
	sf::Vector2f width_and_height = sf::Vector2f(7.5, 1.5) * float(BLOCK_SIZE);
	level.AddComponent<WidthAndHeight>(entity_id)->width_and_height = width_and_height;
	level.AddComponent<Position>(entity_id)->position = position;
	level.AddComponent<FillColor>(entity_id)->color.a = alpha;
	level.AddComponent<Text>(entity_id)->size = 100;
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
EntitiesHandle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesCreator> entities_creators, float spacing)
{
	std::function<EntitiesHandle(EntitiesCreator)> f = [](EntitiesCreator EntitiesCreator) { return EntitiesCreator(sf::Vector2f(0, 0)); };
	std::vector<EntitiesHandle> ids_and_heights = ApplyFuncToVector(entities_creators, f);
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

static EntitiesHandle OneDimensionalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_handles, float spacing, std::function<float&(sf::Vector2f&)> access_layout_direction, std::function<float&(sf::Vector2f&)> access_other_direction, UiOrigin ui_origin)
{
	std::vector<int> total_ids;
	sf::Vector2f total_size = sf::Vector2f(0, 0);

	float& total_size_in_layout_direction = access_layout_direction(total_size);
	float& total_size_in_other_direction = access_other_direction(total_size);

	for (auto [ids, size] : entities_handles)
	{
		if (ids.size() == 0) { continue; }
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
	for (auto [ids, size] : entities_handles)
	{
		if (ids.size() == 0) { continue; }
		current_position_in_layout_direction += access_layout_direction(size) / 2;
		for (int id : ids)
		{
			sf::Vector2f& entity_position = level.GetComponent<Position>(id)->position;
			entity_position += position;
			access_layout_direction(entity_position) += current_position_in_layout_direction;
			total_ids.push_back(id);
		}
		current_position_in_layout_direction += access_layout_direction(size) / 2 + spacing;
	}
	return (std::tuple { total_ids, total_size });
}

EntitiesHandle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_handles, float spacing, UiOrigin ui_origin)
{
	return OneDimensionalEntityLayout(level, position, entities_handles, spacing, GetY, GetX, ui_origin);
}

EntitiesHandle HorizontalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_handles, float spacing, UiOrigin ui_origin)
{
	return OneDimensionalEntityLayout(level, position, entities_handles, spacing, GetX, GetY, ui_origin);
}