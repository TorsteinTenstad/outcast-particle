#include "edit_mode_blueprint_menu_functions.hpp"
#include "systems/_pure_DO_systems.hpp"

static void UpdateUI(Level& level, EditModeUI* ui);
static void SetupUI(Level& level, EditModeUI* ui);

#define UI_BAR_HEIGHT (2 * BLOCK_SIZE)

void EditModeUISystem::Update(Level& level, float dt)
{
	if (level.GetMode() != EDIT_MODE)
	{
		level.ui_bars_size = sf::Vector2f(0, 0);
		return;
	}
	level.ui_bars_size = sf::Vector2f(0, UI_BAR_HEIGHT);
	EditModeUI* ui = level.GetSingleton<EditModeUI>();

	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::B])
	{
		ToggleBlueprintMenu(level);
	}

	if (!ui->initialized)
	{
		SetupUI(level, ui);
		ui->initialized = true;
	}
	UpdateUI(level, ui);
}

static void UpdateUI(Level& level, EditModeUI* ui)
{
}

static void SetupUI(Level& level, EditModeUI* ui)
{
	{
		float w = 3 * BLOCK_SIZE;
		int entity_id = level.AddBlueprint(BPButton);
		level.GetComponent<Position>(entity_id)->position = sf::Vector2f(w / 2, -UI_BAR_HEIGHT / 2);
		level.GetComponent<WidthAndHeight>(entity_id)->width_and_height = sf::Vector2f(w, UI_BAR_HEIGHT);
		level.GetComponent<Text>(entity_id)->content = "+";
		level.GetComponent<Text>(entity_id)->size = 200;
		level.AddComponent<OnReleasedThisFrame>(entity_id)->func = [&level]() { ToggleBlueprintMenu(level); };
	}
}
