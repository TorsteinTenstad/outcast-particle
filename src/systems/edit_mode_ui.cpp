#include "components/button.hpp"
#include "edit_mode_actions/set_property_value_of_selected.hpp"
#include "edit_mode_blueprint_menu_functions.hpp"
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/string_parsing.hpp"

class EditModeUI
{
public:
	bool initialized = false;
};

class EditModeUIEntity
{};

static void UpdateUI(Level& level, EditModeUI* ui);
static void SetupUI(Level& level, EditModeUI* ui);

#define UI_BAR_HEIGHT (2 * BLOCK_SIZE)

void EditModeUISystem::Update(Level& level, float dt)
{
	if (level.GetMode() != EDIT_MODE)
	{
		level.ui_bars_size = sf::Vector2f(0, 0);
		CloseBlueprintMenu(level);
		level.DeleteEntitiesWith<EditModeUIEntity>();
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
	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponent<EditModeUIEntity>(entity); });
	{
		float w = 3 * BLOCK_SIZE;
		sf::Vector2f standard_size = sf::Vector2f(2 * BLOCK_SIZE, 1.5 * BLOCK_SIZE);
		sf::Vector2f narrow_size = sf::Vector2f(1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE);
		auto [add_entity, add_size] = CreateButton(
			level, sf::Vector2f(w / 2, -UI_BAR_HEIGHT / 2), standard_size, [&level]() { ToggleBlueprintMenu(level); }, "+", 200);
		level.AddComponent<ShortcutKey>(add_entity)->key = sf::Keyboard::B;

		auto [undo_entity, undo_size] = CreateCanDisableButton(
			level, sf::Vector2f(3 * w / 2, -UI_BAR_HEIGHT / 2), standard_size, [&]() { level.editor.Undo(); }, "<-", 200, [&]() { return !level.editor.IsEmpty(); });
		auto [redo_entity, redo_size] = CreateCanDisableButton(
			level, sf::Vector2f((5 * w - BLOCK_SIZE) / 2, -UI_BAR_HEIGHT / 2), standard_size, [&]() { level.editor.Redo(); }, "->", 200, [&]() { return !level.editor.IsAtEnd(); });

		for (int i = 0; i < 5; i++)
		{
			auto [increment_button, increment_size] = CreateCanDisableButton(
				level, sf::Vector2f(3.25 * w + 2 * i * BLOCK_SIZE, -UI_BAR_HEIGHT / 2), narrow_size, [&, i]() { level.editor.Do<SetPropertyValueOfSelected>(level, i, std::nullopt); }, ToString(i + 1), 200, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); });
		}
	}
}
