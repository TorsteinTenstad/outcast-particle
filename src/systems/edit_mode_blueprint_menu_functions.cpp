#include "systems/edit_mode_blueprint_menu_functions.hpp"
#include "components/blueprint_menu_item.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/editable.hpp"
#include "components/not_serialized.hpp"
#include "components/position.hpp"
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

void OpenBlueprintMenu(Level& level)
{
	CloseMusicMenu(level);
	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponents<BlueprintMenuItem, NotSerialized>(entity); });

	{ //Background
		auto position = sf::Vector2f(0.5 * BLUEPRINT_MENU_WIDTH, level.GetSize().y / 2.f);
		auto size = sf::Vector2f(BLUEPRINT_MENU_WIDTH, level.GetSize().y);
		auto [entity, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, "content\\textures\\gray.png", false);
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
std::optional<Entity> UpdateBlueprintMenu(Level& level)
{
	for (auto& [entity, pressed_this_frame, blueprint_menu_item, draw_priority, editable] : level.GetEntitiesWith<PressedThisFrame, BlueprintMenuItem, DrawPriority, Editable>())
	{
		draw_priority->draw_priority -= UI_BASE_DRAW_PRIORITY;
		level.RemoveComponents<BlueprintMenuItem, NotSerialized>(entity);
		CloseBlueprintMenu(level);
		return entity;
	}
	return {};
}

void CloseBlueprintMenu(Level& level)
{
	level.DeleteEntitiesWith<BlueprintMenuItem>();
}

void ToggleBlueprintMenu(Level& level)
{
	if (level.GetEntitiesWith<BlueprintMenuItem>().size() > 0)
	{
		CloseBlueprintMenu(level);
	}
	else
	{
		OpenBlueprintMenu(level);
	}
}

//TODO: Should the following functions get their own file, or can these functions be combined with the function above?
//Initial implementation used BlueprintMenuItem - component for both blueprint menu and music menu, but this didn't allow closing one and opening the other simultaneously.
void OpenMusicMenu(Level& level)
{
	CloseBlueprintMenu(level);

	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponents<MusicMenuItem, NotSerialized>(entity); });

	float scale = level.GetScale();
	{ //Background
		auto position = sf::Vector2f(BLUEPRINT_MENU_WIDTH * 2 * scale, level.GetSize().y / 2.f);
		auto size = sf::Vector2f(4 * BLUEPRINT_MENU_WIDTH * scale, level.GetSize().y);
		auto [entity, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, "content\\textures\\gray.png", false);
		level.AddComponents<ReceivesButtonEvents>(entity);
	}
	std::vector<EntitiesHandle> buttons;
	std::string path = "content\\music";

	EntityHandle title = CreateText(level, sf::Vector2f(0, 0), "Select Music", 200 * scale);
	buttons.push_back(ToEntitiesHandle(title));

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string music_path = entry.path().string();

		auto [entity, size] = CreateNavigatorButton(
			level, sf::Vector2f(0, 0), [&, music_path]() { level.music_path = music_path; }, SplitString(music_path, "\\").back(), sf::Keyboard::Unknown, sf::Vector2f(10, 2) * scale * float(BLOCK_SIZE));
		level.AddComponent<StickyButton>(entity);
		if (level.music_path == music_path)
		{
			level.AddComponent<StickyButtonDown>(entity);
		}
		buttons.push_back(ToEntitiesHandle({ entity, size }));
		level.GetComponent<DrawPriority>(entity)->draw_priority += UI_BASE_DRAW_PRIORITY;
	}
	VerticalEntityLayout(level, sf::Vector2f(2 * BLUEPRINT_MENU_WIDTH, BLOCK_SIZE) * scale, buttons, BLOCK_SIZE * scale / 2, StartEdge);
}

void ToggleMusicMenu(Level& level)
{
	if (level.GetEntitiesWith<MusicMenuItem>().size() > 0)
	{
		CloseMusicMenu(level);
	}
	else
	{
		OpenMusicMenu(level);
	}
}

void CloseMusicMenu(Level& level)
{
	level.DeleteEntitiesWith<MusicMenuItem>();
}
