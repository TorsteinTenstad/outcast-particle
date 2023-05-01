#include "systems/edit_mode_blueprint_menu_functions.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "ecs/entity_creation_observer.hpp"
#include "entity_creation.hpp"
#include "utils/container_operations.hpp"
#include "utils/get_size.hpp"

class BlueprintMenuItem
{};

const std::vector<Blueprint> BLUEPRINT_ENTRIES { BPStaticParticle, BPLaser, BPWall, BPElectricField, BPMagneticField, BPCoin };
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

	std::vector<EntitiesHandle> blueprints;
	for (const auto& blueprint : BLUEPRINT_ENTRIES)
	{
		int entity_id = level.AddBlueprint(blueprint);
		sf::Vector2f size = GetSize(level, entity_id, false);
		blueprints.push_back(ToEntitiesHandle({ entity_id, size }));
		level.GetComponent<DrawPriority>(entity_id)->draw_priority += UI_BASE_DRAW_PRIORITY;
	}
	VerticalEntityLayout(level, sf::Vector2f(BLUEPRINT_MENU_WIDTH / 2, BLOCK_SIZE), blueprints, BLOCK_SIZE, StartEdge);
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