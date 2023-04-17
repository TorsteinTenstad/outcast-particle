#include "components/button.hpp"
#include "edit_mode_blueprint_menu_functions.hpp"
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"

class EditModeUI
{
public:
	bool initialized = false;
};

static void UpdateUI(Level& level, EditModeUI* ui);
static void SetupUI(Level& level, EditModeUI* ui);

#define UI_BAR_HEIGHT (2 * BLOCK_SIZE)

void EditModeUISystem::Update(Level& level, float dt)
{
	if (level.GetMode() != EDIT_MODE)
	{
		level.ui_bars_size = sf::Vector2f(0, 0);
		CloseBlueprintMenu(level);
		return;
	}
	level.ui_bars_size = sf::Vector2f(0, UI_BAR_HEIGHT);
	EditModeUI* ui = level.GetSingleton<EditModeUI>();

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
		auto [entity_id, size] = CreateButton(
			level,
			sf::Vector2f(w / 2, -UI_BAR_HEIGHT / 2),
			sf::Vector2f(w, UI_BAR_HEIGHT),
			[&level]() { ToggleBlueprintMenu(level); },
			"+",
			200);
		level.AddComponent<ShortcutKey>(entity_id)->key = sf::Keyboard::B;
	}
}
