#include "components/button.hpp"
#include "components/draw_info.hpp"
#include "components/edit_mode.hpp"
#include "components/physics.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "edit_mode_actions/copy_selected.hpp"
#include "edit_mode_actions/delete_selected.hpp"
#include "edit_mode_actions/modify_level_size.hpp"
#include "edit_mode_actions/resize_selected.hpp"
#include "edit_mode_actions/rotate_selected_fields.hpp"
#include "edit_mode_actions/set_property_value_of_selected.hpp"
#include "edit_mode_blueprint_menu_functions.hpp"
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "systems/edit_mode_blueprint_menu_functions.hpp"
#include "utils/string_parsing.hpp"

void RescaleEditModeUi(Level& level)
{
	float scale_rate = level.GetScale() / level.GetSingleton<EditModeUI>()->level_scale;
	for (auto& [entity, edit_mode_ui_entity] : level.GetEntitiesWith<EditModeUIEntity>())
	{
		level.GetComponent<Position>(entity)->position *= scale_rate;
		level.GetComponent<WidthAndHeight>(entity)->width_and_height *= scale_rate;
		if (Text* text = level.RawGetComponent<Text>(entity))
		{
			text->size *= scale_rate;
		}
	}
}

static void UpdateUI(Level& level, EditModeUI* ui);
static void SetupUI(Level& level, EditModeUI* ui);

#define UI_BAR_HEIGHT (4.25 * BLOCK_SIZE)

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
	if (ui->level_scale != level.GetScale())
	{
		RescaleEditModeUi(level);
		ui->level_scale = level.GetScale();
	}
}

static void SetupUI(Level& level, EditModeUI* ui)
{
	level.DeleteEntitiesWith<EditModeUIEntity>();
	auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponent<EditModeUIEntity>(entity); });
	{
		float scale = level.GetScale();
		float w = 3 * BLOCK_SIZE;
		sf::Vector2f standard_size = sf::Vector2f(2 * BLOCK_SIZE, 1.5 * BLOCK_SIZE);
		sf::Vector2f narrow_size = sf::Vector2f(1.5 * BLOCK_SIZE, 1.5 * BLOCK_SIZE);
		float row_one = -3 * BLOCK_SIZE;
		float row_two = -1.25 * BLOCK_SIZE;

		int default_text_size = 200;

		//Create add-entity-button:
		auto [add_entity, add_size] = CreateButton(
			level, sf::Vector2f(w / 2, row_one), standard_size, [&level]() { ToggleBlueprintMenu(level); }, "+", default_text_size);
		level.AddComponent<ShortcutKey>(add_entity)->key = sf::Keyboard::B;

		//Create delete-button:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(2.5 * w / 2, row_one), standard_size, [&level]() { level.editor.Do<DeleteSelected>(level); }, "content\\textures\\delete.png", 200, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); });

		//Create undo- and redo-buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(w / 2, row_two), standard_size, [&]() { level.editor.Undo(); }, "content\\textures\\undo.png", default_text_size, [&]() { return !level.editor.IsEmpty(); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(2.5 * w / 2, row_two), standard_size, [&]() { level.editor.Redo(); }, "content\\textures\\redo.png", default_text_size, [&]() { return !level.editor.IsAtEnd(); });

		//Create property value buttons:

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(7.875 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, 0, std::nullopt); }, "content\\textures\\1.png", default_text_size, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(9.625 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, 1, std::nullopt); }, "content\\textures\\2.png", default_text_size, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(11.375 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, 2, std::nullopt); }, "content\\textures\\3.png", default_text_size, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(8.75 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, 3, std::nullopt); }, "content\\textures\\4.png", default_text_size, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(10.5 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, 4, std::nullopt); }, "content\\textures\\5.png", default_text_size, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); });

		//Create resize buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(15.25 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(1, 0)); }, "content\\textures\\widen.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(17 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(0, 1)); }, "content\\textures\\heighten.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(15.25 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(-1, 0)); }, "content\\textures\\narrow.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(17 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(0, -1)); }, "content\\textures\\shorten.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0); });

		//Create rotate-buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<std::string> icon_paths = { "content\\textures\\rotate_left.png", "content\\textures\\rotate_right.png" };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(20.875 * BLOCK_SIZE + 1.75 * i * BLOCK_SIZE, row_one), narrow_size, [&, i]() { level.editor.Do<RotateSelectedFields>(level, PI / 2 * (-1 + 2 * i)); }, icon_paths[i], default_text_size, [&]() { return (level.GetEntitiesWith<Selected, ElectricField>().size() != 0); });
		}

		//Create invert charge button:

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(21.75 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, std::nullopt, -1); }, "content\\textures\\flip.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, MagneticField>().size() != 0 || level.GetEntitiesWith<Selected, Charge>().size() != 0); });

		//Create level size-buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<int> increment = { 1, -1 };
			std::vector<std::string> icon_paths = { "content\\textures\\increase_size.png", "content\\textures\\decrease_size.png" };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(26.5 * BLOCK_SIZE, -UI_BAR_HEIGHT / 2 - 0.875 * BLOCK_SIZE * increment[i]), standard_size, [&, increment, i]() { level.editor.Do<ModifyLevelSize>(level, increment[i]); }, icon_paths[i], default_text_size, [&, increment, i]() { return (level.GetValidNewSizeId(increment[i]) != 0); });
		}

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(30.75 * BLOCK_SIZE, row_two), narrow_size, [&]() { ToggleMusicMenu(level); }, "content\\textures\\music.png", default_text_size, []() { return true; });

		Entity help_menu_entity = GetEntity(CreateMouseEventButton(
			level, sf::Vector2f(30.75 * BLOCK_SIZE, row_one), narrow_size));
		level.AddComponent<Text>(help_menu_entity)->content = "?";
		level.GetComponent<Text>(help_menu_entity)->size = default_text_size;
		level.AddComponent<ShowHelpMenuButton>(help_menu_entity);
	}
}
