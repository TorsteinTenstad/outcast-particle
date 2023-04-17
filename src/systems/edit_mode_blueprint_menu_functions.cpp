#include "systems/edit_mode_blueprint_menu_functions.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "ecs/entity_creation_observer.hpp"
#include "entity_creation.hpp"

class BlueprintMenuItem
{};

const std::vector<Blueprint> BLUEPRINT_ENTRIES { BPGoal, BPStaticParticle, BPLaser, BPWall, BPElectricField, BPMagneticField, BPCoin };
#define BLUEPRINT_MENU_WIDTH (3 * BLOCK_SIZE)

void OpenBlueprintMenu(Level& level)
{
	auto e = EntityCreationObserver(level, [](ECSScene& level, int id) { level.AddComponent<BlueprintMenuItem>(id); });

	{ //Background
		auto position = sf::Vector2f(0.5 * BLUEPRINT_MENU_WIDTH, level.GetSize().y / 2.f);
		auto size = sf::Vector2f(BLUEPRINT_MENU_WIDTH, level.GetSize().y);
		auto [id, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, "content\\textures\\gray.png", false);
		level.AddComponent<ReceivesButtonEvents>(id);
	}

	int i = 0;
	for (const auto& blueprint : BLUEPRINT_ENTRIES)
	{
		int entity_id = level.AddBlueprint(blueprint);
		level.GetComponent<Position>(entity_id)->position = sf::Vector2f(0.5 * BLUEPRINT_MENU_WIDTH, (2 + 2 * i + (i > 0 ? 1 : 0)) * BLOCK_SIZE);
		level.GetComponent<DrawPriority>(entity_id)->draw_priority += UI_BASE_DRAW_PRIORITY;
		i++;
	}
}

//Handle selection from blueprint menu. Return any selected id.
std::optional<int> UpdateBlueprintMenu(Level& level)
{
	for (auto& [entity_id, pressed_this_frame, blueprint_menu_item, draw_priority] : level.GetEntitiesWith<PressedThisFrame, BlueprintMenuItem, DrawPriority>())
	{
		draw_priority->draw_priority -= UI_BASE_DRAW_PRIORITY;
		level.RemoveComponents<BlueprintMenuItem>(entity_id);
		CloseBlueprintMenu(level);
		return entity_id;
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