#include "button.hpp"
#include "components/button_events.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"

void HelpMenuSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != EDIT_MODE)
	{
		level.DeleteEntitiesWith<ShowHelpMenuButton>();
		return;
	}
	sf::Vector2f level_size = level.GetSize();
	// float scale = level.GetScale();
	// std::function<Entity(ECSScene&)> button_creation_func = [](ECSScene& level) {
	// 	Entity button_entity = GetEntity(CreateMouseEventButton(level, {}, sf::Vector2f(1.5, 1.5) * float(BLOCK_SIZE)));
	// 	level.AddComponent<Text>(button_entity)->content = "?";
	// 	return button_entity;
	// };
	// auto [button_entity, show_help_menu_button] = level.GetSingletonIncludeID<ShowHelpMenuButton>(button_creation_func);
	// level.GetComponent<Position>(button_entity)->position = sf::Vector2f(31 * BLOCK_SIZE, -BLOCK_SIZE) * scale;

	std::optional<Entity> opt_button_entity = level.FindSingletonId<ShowHelpMenuButton>();

	if (!opt_button_entity)
	{
		return;
	}

	Entity button_entity = opt_button_entity.value();
	bool button_pressed = level.HasComponents<ReleasedThisFrame>(button_entity);
	bool& menu_open = level.GetComponent<ShowHelpMenuButton>(button_entity)->menu_open;

	if (menu_open && (button_pressed || cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape]))
	{
		level.GetComponent<Text>(button_entity)->content = "?";
		DeleteChildrenOwnedBy<ShowHelpMenuButton>(level, button_entity);
		menu_open = !menu_open;
		cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape] = false;
	}
	else if (!menu_open && button_pressed)
	{
		level.GetComponent<Text>(button_entity)->content = "X";
		std::function<Entity(ECSScene&)> menu_creation_func = [level_size](ECSScene& level) { return GetEntity(CreateScreenWideBlur(level, level_size, UI_BASE_DRAW_PRIORITY - 1)); };
		GetSingletonChildId<ShowHelpMenuButton>(level, button_entity, menu_creation_func);
		menu_open = !menu_open;
	}
}