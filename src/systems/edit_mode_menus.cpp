#include "systems/edit_mode_menus.hpp"
#include "components/blueprint_menu_item.hpp"
#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/edit_mode.hpp"
#include "components/editable.hpp"
#include "components/not_serialized.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/scale_with_level.hpp"
#include "components/size.hpp"
#include "components/sticky_button.hpp"
#include "ecs/entity_creation_observer.hpp"
#include "entity_creation.hpp"
#include "utils/container_operations.hpp"
#include "utils/get_size.hpp"
#include "utils/string_parsing.hpp"
#include <filesystem>
#include <iostream>

const std::vector<Blueprint> BLUEPRINT_ENTRIES { BPStaticParticle, BPMovingParticle, BPLaser, BPWall, BPElectricField, BPMagneticField, BPCoin };
#define BLUEPRINT_MENU_WIDTH (3 * BLOCK_SIZE)

void BlueprintMenu::Create(Level& level)
{
	HelpMenu().Close(level);
	auto level_size = level.GetSize();
	{ //Background
		auto position = sf::Vector2f(0.5 * BLUEPRINT_MENU_WIDTH, level_size.y / 2.f);
		auto size = sf::Vector2f(BLUEPRINT_MENU_WIDTH, level_size.y);
		auto [entity, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, (TEXTURES_DIR / "gray.png").string(), false);
		level.AddComponents<ReceivesButtonEvents>(entity);
	}

	std::vector<EntitiesHandle> blueprints;
	for (const auto& blueprint : BLUEPRINT_ENTRIES)
	{
		Entity entity = level.AddBlueprint(blueprint);
		sf::Vector2f size = GetSize(level, entity, false);
		blueprints.push_back(ToEntitiesHandle({ entity, size }));
		level.GetComponent<DrawPriority>(entity)->draw_priority += UI_BASE_DRAW_PRIORITY;
	}
	VerticalEntityLayout(level, sf::Vector2f(BLUEPRINT_MENU_WIDTH / 2, BLOCK_SIZE), blueprints, BLOCK_SIZE, StartEdge);
}

//Handle selection from blueprint menu. Return any selected entity.
std::optional<Entity> BlueprintMenu::Update(Level& level)
{
	for (auto& [entity, pressed_this_frame, blueprint_menu_item, draw_priority, editable] : level.GetEntitiesWith<PressedThisFrame, BlueprintMenuItem, DrawPriority, Editable>())
	{
		draw_priority->draw_priority -= UI_BASE_DRAW_PRIORITY;
		level.RemoveComponents<BlueprintMenuItem, NotSerialized>(entity);
		Close(level);
		return entity;
	}
	return std::nullopt;
}

void MusicMenu::Create(Level& level)
{
	HelpMenu().Close(level);
	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponent<ScaleWithLevel>(entity); });

	float scale = level.GetScale();
	auto level_size = level.GetSize();
	{ //Background
		auto position = sf::Vector2f(level_size.x - BLUEPRINT_MENU_WIDTH * 2 * scale, level_size.y / 2.f);
		auto size = sf::Vector2f(4 * BLUEPRINT_MENU_WIDTH * scale, level_size.y);
		auto [entity, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY - 1, (TEXTURES_DIR / "gray.png").string(), false);
		level.AddComponents<ReceivesButtonEvents>(entity);
	}
	std::vector<EntitiesHandle> buttons;

	EntityHandle player_option_title = CreateText(level, sf::Vector2f(0, 0), "Player Controls", 100 * scale);
	buttons.push_back(ToEntitiesHandle(player_option_title));

	auto [button_1, size_1] = CreateButton(
		level, sf::Vector2f(0, 0), sf::Vector2f(10, 1) * scale * float(BLOCK_SIZE), [&]() { level.GetSingleton<Player>()->can_switch_charge = !level.GetSingleton<Player>()->can_switch_charge; }, "Switch Charge", 80);
	level.AddComponent<StickyButton>(button_1);
	if (level.GetSingleton<Player>()->can_switch_charge) { level.AddComponent<StickyButtonDown>(button_1); }
	buttons.push_back(ToEntitiesHandle({ button_1, size_1 }));

	auto [button_2, size_2] = CreateButton(
		level, sf::Vector2f(0, 0), sf::Vector2f(10, 1) * scale * float(BLOCK_SIZE), [&]() { level.GetSingleton<Player>()->can_go_neutral = !level.GetSingleton<Player>()->can_go_neutral; }, "Go Neutral", 80);
	level.AddComponent<StickyButton>(button_2);
	if (level.GetSingleton<Player>()->can_switch_charge) { level.AddComponent<StickyButtonDown>(button_2); }
	buttons.push_back(ToEntitiesHandle({ button_2, size_2 }));

	auto [button_3, size_3] = CreateButton(
		level, sf::Vector2f(0, 0), sf::Vector2f(10, 1) * scale * float(BLOCK_SIZE), [&]() { level.GetSingleton<Player>()->move_force = 0; }, "WASD", 80);
	level.AddComponent<StickyButton>(button_3);
	if (!level.GetSingleton<Player>()->move_force == 0) { level.AddComponent<StickyButtonDown>(button_3); }
	buttons.push_back(ToEntitiesHandle({ button_3, size_3 }));

	auto entity_copy = button_3;

	auto [slider_entities, size_4] = CreateSliderButton(level, sf::Vector2f(0, 0), sf::Vector2f(8, 1) * scale * float(BLOCK_SIZE), &level.GetSingleton<Player>()->move_force, { 0, 1000 });
	for (auto entity : slider_entities)
	{
		level.AddComponent<CanDisableButton>(entity)->func = [&, entity_copy]() { return level.HasComponents<StickyButtonDown>(entity_copy); };
		if (!level.HasComponents<SliderButton>(entity)) { level.GetComponent<CanDisableButton>(entity)->regain_button_events = false; }
	}
	buttons.push_back({ slider_entities, size_4 });

	EntityHandle music_title = CreateText(level, sf::Vector2f(0, 0), "Select Music", 100 * scale);
	buttons.push_back(ToEntitiesHandle(music_title));

	for (const auto& entry : std::filesystem::directory_iterator(MUSIC_DIR))
	{
		std::filesystem::path music_fspath = entry.path();
		std::string music_path = music_fspath.string();

		auto [entity, size] = CreateButton(
			level, sf::Vector2f(0, 0), sf::Vector2f(10, 1) * scale * float(BLOCK_SIZE), [&, music_path]() { level.music_path = music_path; }, music_fspath.filename().string(), 80);
		level.AddComponent<StickyButton>(entity)->enforce_down = true;
		level.GetComponent<StickyButton>(entity)->channel = 1;
		if (level.music_path == music_path)
		{
			level.AddComponent<StickyButtonDown>(entity);
		}
		buttons.push_back(ToEntitiesHandle({ entity, size }));
		level.GetComponent<DrawPriority>(entity)->draw_priority += 1;
	}

	VerticalEntityLayout(level, sf::Vector2f(level_size.x - 2 * BLUEPRINT_MENU_WIDTH * scale, BLOCK_SIZE * scale), buttons, BLOCK_SIZE * scale / 4, StartEdge);
}

static EntitiesHandle CreateIcon(Level& level, std::string image_path)
{
	sf::Vector2f width_and_height = sf::Vector2f(1, 1) * BLOCK_SIZE;
	Entity icon = GetEntity(level.CreateEntityWith<Position>());
	level.AddComponent<DrawInfo>(icon)->image_path = image_path;
	level.AddComponent<DrawPriority>(icon)->draw_priority = 2 * UI_BASE_DRAW_PRIORITY + 1;
	level.AddComponent<WidthAndHeight>(icon)->width_and_height = width_and_height;

	Entity button = GetEntity(level.CreateEntityWith<Position, DrawInfo>());
	level.AddComponent<DrawPriority>(button)->draw_priority = 2 * UI_BASE_DRAW_PRIORITY;
	level.AddComponent<WidthAndHeight>(button)->width_and_height = width_and_height;
	level.AddComponent<Shader>(button)->fragment_shader_path = (SHADERS_DIR / "round_corners.frag").string();
	level.AddComponent<FillColor>(button)->color = DEFAULT_COLOR;

	return { { icon, button }, width_and_height };
}

void HelpMenu::Create(Level& level)
{
	BlueprintMenu().Close(level);
	MusicMenu().Close(level);
	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponent<ScaleWithLevel>(entity); });
	auto level_size = level.GetSize();
	CreateScreenWideBlur(level, level_size, UI_BASE_DRAW_PRIORITY - 1);

	std::vector<EntitiesHandle> icon_handles = {};
	std::vector<EntitiesHandle> explanation_handles = {};

	std::vector<std::tuple<std::string, std::vector<std::string>>> row_data = {
		{ "Add an element to the level", { (TEXTURES_DIR / "add.png").string() } },
		{ "Delete all selected elements", { (TEXTURES_DIR / "delete_entity.png").string() } },
		{ "Undo and redo", { (TEXTURES_DIR / "undo.png").string(), (TEXTURES_DIR / "redo.png").string() } },
		{ "Increase or decrease level size", { (TEXTURES_DIR / "increase_size.png").string(), (TEXTURES_DIR / "decrease_size.png").string() } },
		{ "Change width and height of element", { (TEXTURES_DIR / "widen.png").string(), (TEXTURES_DIR / "narrow.png").string(), (TEXTURES_DIR / "heighten.png").string(), (TEXTURES_DIR / "shorten.png").string() } },
		{ "Set strength of charges and fields, bounciness of walls and timing of lasers", { (TEXTURES_DIR / "1.png").string(), (TEXTURES_DIR / "2.png").string(), (TEXTURES_DIR / "3.png").string(), (TEXTURES_DIR / "4.png").string(), (TEXTURES_DIR / "5.png").string() } },
		{ "Increase and decrease particle velocity", { (TEXTURES_DIR / "increase_velocity.png").string(), (TEXTURES_DIR / "decrease_velocity.png").string() } },
		{ "Change direction of particle velocity", { (TEXTURES_DIR / "rotate_right.png").string(), (TEXTURES_DIR / "rotate_left.png").string() } },
		{ "Change direction of electronic fields", { (TEXTURES_DIR / "rotate_90.png").string(), (TEXTURES_DIR / "counter_rotate_90.png").string() } },
		{ "Flip direction of magnetic fields, and sign of particle charge", { (TEXTURES_DIR / "flip.png").string() } },
		{ "Select track", { (TEXTURES_DIR / "music.png").string() } },
		{ "Open and close this help screen", { (TEXTURES_DIR / "help.png").string() } },
	};

	for (const auto& [explanation, icon_paths] : row_data)
	{
		std::vector<EntitiesHandle> icons = {};
		for (const auto& icon_path : icon_paths)
		{
			icons.emplace_back(CreateIcon(level, icon_path));
		}
		EntitiesHandle icons_handle = HorizontalEntityLayout(level, sf::Vector2f(0, 0), icons, 0.25 * BLOCK_SIZE, UiOrigin::EndEdge);
		EntitiesHandle explanation_handle = ToEntitiesHandle(CreateText(level, sf::Vector2f(0, 0), explanation, 100, sf::Vector2f(1, 1) * BLOCK_SIZE, TextOrigin::REFERENCE_HEIGHT_LEFT));
		icon_handles.push_back(icons_handle);
		explanation_handles.push_back(explanation_handle);
	}
	float y = 7 * BLOCK_SIZE;
	float column_margin = 0.25 * BLOCK_SIZE;
	float row_margin = 0.5 * BLOCK_SIZE;
	VerticalEntityLayout(level, sf::Vector2f(y, level_size.y / 2.f), icon_handles, row_margin);
	VerticalEntityLayout(level, sf::Vector2f(y + column_margin, level_size.y / 2.f), explanation_handles, row_margin);
}