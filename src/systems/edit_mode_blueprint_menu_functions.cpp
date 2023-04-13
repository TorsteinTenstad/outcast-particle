#include "systems/edit_mode_blueprint_menu_functions.hpp"
#include "ecs/entity_creation_observer.hpp"

const std::vector<Blueprint> BLUEPRINT_ENTRIES { BPGoal, BPStaticParticle, BPLaser, BPWall, BPElectricField, BPMagneticField, BPCoin };
#define BLUEPRINT_MENU_WIDTH (3 * BLOCK_SIZE)
void OpenBlueprintMenu(Level& level)
{
	auto e = EntityCreationObserver(level, [](ECSScene& level, int id) { level.AddComponent<BlueprintMenuItem>(id); });

	int i = 0;
	int menu_background_id = level.CreateEntityId();
	level.AddComponent<Position>(menu_background_id)->position = sf::Vector2f(0.5 * BLUEPRINT_MENU_WIDTH, level.GetSize().y / 2.f);
	level.AddComponent<DrawInfo>(menu_background_id)->image_path = "content\\textures\\gray.png";
	level.AddComponent<DrawPriority>(menu_background_id)->draw_priority = UI_BASE_DRAW_PRIORITY;
	level.AddComponent<ReceivesButtonEvents>(menu_background_id);
	level.AddComponent<WidthAndHeight>(menu_background_id)->width_and_height = sf::Vector2f(BLUEPRINT_MENU_WIDTH, level.GetSize().y);
	int entity_id;
	for (const auto& tag : BLUEPRINT_ENTRIES)
	{
		entity_id = level.AddBlueprint(tag);
		level.GetComponent<Position>(entity_id)->position = sf::Vector2f(0.5 * BLUEPRINT_MENU_WIDTH, (2 + 2 * i + (i > 0 ? 1 : 0)) * BLOCK_SIZE);
		level.GetComponent<DrawPriority>(entity_id)->draw_priority += UI_BASE_DRAW_PRIORITY;
		i++;
	}
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