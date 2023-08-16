#include "components/button.hpp"
#include "components/edit_mode.hpp"
#include "components/physics.hpp"
#include "components/text.hpp"
#include "edit_mode_actions/modify_level_size.hpp"
#include "edit_mode_actions/rotate_selected_fields.hpp"
#include "edit_mode_actions/set_property_value_of_selected.hpp"
#include "edit_mode_blueprint_menu_functions.hpp"
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "systems/edit_mode_blueprint_menu_functions.hpp"
#include "utils/string_parsing.hpp"

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
	level.ui_bars_size = sf::Vector2f(0, UI_BAR_HEIGHT) * level.GetScale();
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
	level.DeleteEntitiesWith<EditModeUIEntity>();
	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponent<EditModeUIEntity>(entity); });
	{
		float scale = level.GetScale();
		float w = 3 * BLOCK_SIZE;
		sf::Vector2f standard_size = sf::Vector2f(2 * BLOCK_SIZE, 1.5 * BLOCK_SIZE) * scale;
		sf::Vector2f narrow_size = sf::Vector2f(1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE) * scale;
		int default_text_size = 200 * scale;

		//Create add-entity-button:
		auto [add_entity, add_size] = CreateButton(
			level, sf::Vector2f(w / 2, -UI_BAR_HEIGHT / 2) * scale, standard_size, [&level]() { ToggleBlueprintMenu(level); }, "+", default_text_size);
		level.AddComponent<ShortcutKey>(add_entity)->key = sf::Keyboard::B;

		//Create undo- and redo-buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(3 * w / 2, -UI_BAR_HEIGHT / 2) * scale, standard_size, [&]() { level.editor.Undo(); }, "content\\textures\\undo.png", default_text_size, [&]() { return !level.editor.IsEmpty(); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f((5 * w - 1.5 * BLOCK_SIZE) / 2, -UI_BAR_HEIGHT / 2) * scale, standard_size, [&]() { level.editor.Redo(); }, "content\\textures\\redo.png", default_text_size, [&]() { return !level.editor.IsAtEnd(); });

		//Create property value buttons:
		for (int i = 0; i < 5; i++)
		{
			std::vector<std::string> icon_paths = {
				"content\\textures\\1.png",
				"content\\textures\\2.png",
				"content\\textures\\3.png",
				"content\\textures\\4.png",
				"content\\textures\\5.png",
			};
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(9.5 / 3 * w + 1.75 * i * BLOCK_SIZE, -UI_BAR_HEIGHT / 2) * scale, narrow_size, [&, i]() { level.editor.Do<SetPropertyValueOfSelected>(level, i, std::nullopt); }, icon_paths[i], default_text_size, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); });
		}

		//Create rotate-buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<std::string> icon_paths = { "content\\textures\\rotate_left.png", "content\\textures\\rotate_right.png" };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(19 * BLOCK_SIZE + 1.75 * i * BLOCK_SIZE, -UI_BAR_HEIGHT / 2) * scale, narrow_size, [&, i]() { level.editor.Do<RotateSelectedFields>(level, PI / 2 * (-1 + 2 * i)); }, icon_paths[i], default_text_size, [&]() { return (level.GetEntitiesWith<Selected, ElectricField>().size() != 0); });
		}

		//Create level size-buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<int> increment = { 1, -1 };
			std::vector<std::string> icon_paths = { "content\\textures\\increase_size.png", "content\\textures\\decrease_size.png" };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(23.5 * BLOCK_SIZE + 2.25 * i * BLOCK_SIZE, -UI_BAR_HEIGHT / 2) * scale, standard_size, [&, increment, i]() {level.editor.Do<ModifyLevelSize>(level, increment[i]); globals.skip_drawing_this_frame = true; }, icon_paths[i], default_text_size, [&, increment, i]() { return (level.GetValidNewSizeId(increment[i]) != 0); });
		}

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(28.75 * BLOCK_SIZE, -UI_BAR_HEIGHT / 2) * scale, standard_size, [&]() { ToggleMusicMenu(level); }, "content\\textures\\music.png", default_text_size, []() { return false; });

		Entity help_menu_entity = GetEntity(CreateMouseEventButton(
			level, sf::Vector2f(31 * BLOCK_SIZE, -UI_BAR_HEIGHT / 2) * scale, narrow_size));
		level.AddComponent<Text>(help_menu_entity)->content = "?";
		level.GetComponent<Text>(help_menu_entity)->size = default_text_size;
		level.AddComponent<ShowHelpMenuButton>(help_menu_entity);
	}
}
