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
#include "components/tooltip.hpp"
#include "ecs/entity_creation_observer.hpp"
#include "entity_creation.hpp"
#include "utils/container_operations.hpp"
#include "utils/get_size.hpp"
#include "utils/string_manip.hpp"
#include "utils/string_parsing.hpp"
#include <filesystem>
#include <iostream>

const std::vector<Blueprint> BLUEPRINT_ENTRIES { BPStaticParticle, BPMovingParticle, BPLaser, BPWall, BPElectricField, BPMagneticField, BPCoin };
#define BLUEPRINT_MENU_WIDTH (3 * BLOCK_SIZE)

void BlueprintMenu::Create(Level& level)
{
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

std::optional<Entity> BlueprintMenu::HandleSelection(Level& level)
{
	for (auto& [entity, pressed_this_frame, blueprint_menu_item, draw_priority, editable] : level.GetEntitiesWith<PressedThisFrame, BlueprintMenuItem, DrawPriority, Editable>())
	{
		draw_priority->draw_priority -= UI_BASE_DRAW_PRIORITY;
		level.RemoveComponents<BlueprintMenuItem, NotSerialized, EditModeUIEntity>(entity);
		Close(level);
		return entity;
	}
	return std::nullopt;
}

void MusicMenu::Create(Level& level)
{
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

	EntityHandle music_title = CreateText(level, sf::Vector2f(0, 0), "Select Music", 100 * scale);
	buttons.push_back(ToEntitiesHandle(music_title));

	for (const auto& entry : std::filesystem::directory_iterator(MUSIC_DIR))
	{
		std::filesystem::path music_fspath = entry.path();
		std::string music_path = music_fspath.string();

		auto [entity, size] = CreateButton(
			level, sf::Vector2f(0, 0), sf::Vector2f(10, 1.5) * scale * float(BLOCK_SIZE), [&, music_path]() { level.music_path = music_path; }, music_fspath.filename().string(), 80);
		level.AddComponent<StickyButton>(entity)->enforce_down = true;
		level.GetComponent<StickyButton>(entity)->channel = 1;
		if (level.music_path == music_path)
		{
			level.AddComponent<StickyButtonDown>(entity);
		}
		buttons.push_back(ToEntitiesHandle({ entity, size }));
		level.GetComponent<DrawPriority>(entity)->draw_priority += 1;
	}

	VerticalEntityLayout(level, sf::Vector2f(level_size.x - 2 * BLUEPRINT_MENU_WIDTH * scale, BLOCK_SIZE * scale), buttons, BLOCK_SIZE * scale / 2, StartEdge);
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

void PlayerMenu::Create(Level& level)
{
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
		level, sf::Vector2f(0, 0), sf::Vector2f(10, 1.5) * scale * float(BLOCK_SIZE), [&]() { level.GetSingleton<Player>()->can_switch_charge = !level.GetSingleton<Player>()->can_switch_charge; }, "Switch Charge", 80);
	level.AddComponent<StickyButton>(button_1);
	Tooltip* tooltip_1 = level.AddComponent<Tooltip>(button_1);
	tooltip_1->text = "Toggle the players ability to switch charge using " + HumanName(globals.key_config.PLAYER_SWITCH_CHARGE);
	tooltip_1->preferred_corner = PreferredTooltipCorner::TOP_RIGHT;
	if (level.GetSingleton<Player>()->can_switch_charge) { level.AddComponent<StickyButtonDown>(button_1); }
	buttons.push_back(ToEntitiesHandle({ button_1, size_1 }));

	auto [button_2, size_2] = CreateButton(
		level, sf::Vector2f(0, 0), sf::Vector2f(10, 1.5) * scale * float(BLOCK_SIZE), [&]() { level.GetSingleton<Player>()->can_go_neutral = !level.GetSingleton<Player>()->can_go_neutral; }, "Go Neutral", 80);
	level.AddComponent<StickyButton>(button_2);
	Tooltip* tooltip_2 = level.AddComponent<Tooltip>(button_2);
	tooltip_2->text = "Toggle the players ability to go neutral using " + HumanName(globals.key_config.PLAYER_GO_NEUTRAL);
	tooltip_2->preferred_corner = PreferredTooltipCorner::TOP_RIGHT;
	if (level.GetSingleton<Player>()->can_switch_charge) { level.AddComponent<StickyButtonDown>(button_2); }
	buttons.push_back(ToEntitiesHandle({ button_2, size_2 }));

	auto button_3_handle = CreateButton(
		level, sf::Vector2f(0, 0), sf::Vector2f(10, 1.5) * scale * float(BLOCK_SIZE), [&]() { level.GetSingleton<Player>()->can_move_self = !level.GetSingleton<Player>()->can_move_self; }, "Direct force", 80);
	auto button_3 = GetEntity(button_3_handle);
	level.AddComponent<StickyButton>(button_3);
	Tooltip* tooltip_3 = level.AddComponent<Tooltip>(button_3);
	std::string wasd_string = HumanName(globals.key_config.PLAYER_MOVE_UP) + ", " + HumanName(globals.key_config.PLAYER_MOVE_LEFT) + ", " + HumanName(globals.key_config.PLAYER_MOVE_DOWN) + " and " + HumanName(globals.key_config.PLAYER_MOVE_RIGHT);
	tooltip_3->text = "Toggle the players ability to move using  " + wasd_string;
	tooltip_3->preferred_corner = PreferredTooltipCorner::TOP_RIGHT;
	if (level.GetSingleton<Player>()->can_move_self) { level.AddComponent<StickyButtonDown>(button_3); }

	auto entity_copy = button_3;

	auto slider_handle = CreateSliderButton(level, sf::Vector2f(0, 0), sf::Vector2f(8, 1) * scale * float(BLOCK_SIZE), &level.GetSingleton<Player>()->move_force, { 0, 5000 });
	auto slider_entities = GetEntities(slider_handle);
	Tooltip* slider_tooltip = level.AddComponent<Tooltip>(slider_entities[0]);
	slider_tooltip->text = "Adjust strength of force applied by " + wasd_string;
	slider_tooltip->preferred_corner = PreferredTooltipCorner::TOP_RIGHT;
	for (auto entity : slider_entities)
	{
		CanDisableButton* can_disable_button = level.AddComponent<CanDisableButton>(entity);
		can_disable_button->func = [&, entity_copy]() { return level.GetSingleton<Player>()->can_move_self; };
		can_disable_button->deactivation_alpha = 0;
		if (!level.HasComponents<SliderButton>(entity)) { level.GetComponent<CanDisableButton>(entity)->regain_button_events = false; }
	}
	buttons.push_back(VerticalEntityLayout(level, sf::Vector2f(0, 0), { ToEntitiesHandle(button_3_handle), slider_handle }, BLOCK_SIZE * scale / 4));
	VerticalEntityLayout(level, sf::Vector2f(level_size.x - 2 * BLUEPRINT_MENU_WIDTH * scale, BLOCK_SIZE * scale), buttons, BLOCK_SIZE * scale, StartEdge);
}