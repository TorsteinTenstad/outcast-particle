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

void BlueprintMenu::Create(Level& level)
{
	MusicMenu().Close(level);

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
std::optional<Entity> BlueprintMenu::Update(Level& level)
{
	for (auto& [entity, pressed_this_frame, blueprint_menu_item, draw_priority, editable] : level.GetEntitiesWith<PressedThisFrame, BlueprintMenuItem, DrawPriority, Editable>())
	{
		draw_priority->draw_priority -= UI_BASE_DRAW_PRIORITY;
		level.RemoveComponents<BlueprintMenuItem, NotSerialized>(entity);
		Close(level);
		return entity;
	}
	return {};
}

void MusicMenu::Create(Level& level)
{
	BlueprintMenu().Close(level);

	{ //Background
		auto position = sf::Vector2f(BLUEPRINT_MENU_WIDTH, level.GetSize().y / 2.f);
		auto size = sf::Vector2f(2 * BLUEPRINT_MENU_WIDTH, level.GetSize().y);
		auto [entity, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, "content\\textures\\gray.png", false);
		level.AddComponents<ReceivesButtonEvents>(entity);
	}

	std::vector<EntitiesHandle> buttons;
	std::string path = "content\\music";

	EntityHandle title = CreateText(level, sf::Vector2f(0, 0), "Select Music", 100);
	buttons.push_back(ToEntitiesHandle(title));

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		std::string music_path = entry.path().string();

		auto [entity, size] = CreateNavigatorButton(
			level, sf::Vector2f(0, 0), [&, music_path]() { level.music_path = music_path; }, SplitString(music_path, "\\").back(), sf::Keyboard::Unknown, sf::Vector2f(5, 1) * level.GetScale() * float(BLOCK_SIZE));
		level.AddComponent<StickyButton>(entity);
		if (level.music_path == music_path)
		{
			level.AddComponent<StickyButtonDown>(entity);
		}
		buttons.push_back(ToEntitiesHandle({ entity, size }));
		level.GetComponent<DrawPriority>(entity)->draw_priority += UI_BASE_DRAW_PRIORITY;
	}
	VerticalEntityLayout(level, sf::Vector2f(BLUEPRINT_MENU_WIDTH, BLOCK_SIZE), buttons, BLOCK_SIZE / 2, StartEdge);
}