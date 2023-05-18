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

const std::vector<Blueprint> BLUEPRINT_ENTRIES { BPStaticParticle, BPMovingParticle, BPLaser, BPWall, BPElectricField, BPMagneticField, BPCoin };
#define BLUEPRINT_MENU_WIDTH (3 * BLOCK_SIZE)

void OpenBlueprintMenu(Level& level)
{
	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponent<BlueprintMenuItem>(entity); });

	{ //Background
		auto position = sf::Vector2f(0.5 * BLUEPRINT_MENU_WIDTH, level.GetSize().y / 2.f);
		auto size = sf::Vector2f(BLUEPRINT_MENU_WIDTH, level.GetSize().y);
		auto [entity, _] = CreateTexturedRectangle(level, position, size, UI_BASE_DRAW_PRIORITY, "content\\textures\\gray.png", false);
		level.AddComponent<ReceivesButtonEvents>(entity);
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
	for (auto& [entity, pressed_this_frame, blueprint_menu_item, draw_priority] : level.GetEntitiesWith<PressedThisFrame, BlueprintMenuItem, DrawPriority>())
	{
		draw_priority->draw_priority -= UI_BASE_DRAW_PRIORITY;
		level.RemoveComponents<BlueprintMenuItem>(entity);
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