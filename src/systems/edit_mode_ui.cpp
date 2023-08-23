#include "components/blueprint_menu_item.hpp"
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

void RescaleEditModeUi(Level& level, EditModeUI* ui)
{
	float level_scale = level.GetScale();
	float scale_rate = level_scale / ui->level_scale;
	ui->level_scale = level.GetScale();
	for (auto& [entity, edit_mode_ui_entity] : level.GetEntitiesWith<EditModeUIEntity>())
	{
		if (level.HasComponents<BlueprintMenuItem>(entity)) { continue; }
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
		RescaleEditModeUi(level, ui);
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
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(1.25 * BLOCK_SIZE, row_one), narrow_size, [&level]() { BlueprintMenu().Toggle(level); }, "content\\textures\\add.png", default_text_size, []() { return true; });

		//Create delete-button:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(w, row_one), narrow_size, [&level]() { level.editor.Do<DeleteSelected>(level); }, "content\\textures\\delete_entity.png", 200, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });

		//Create undo- and redo-buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(1.25 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Undo(); }, "content\\textures\\undo.png", default_text_size, [&]() { return !level.editor.IsEmpty() && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(w, row_two), narrow_size, [&]() { level.editor.Redo(); }, "content\\textures\\redo.png", default_text_size, [&]() { return !level.editor.IsAtEnd() && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });

		//Create resize buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(6.45 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(1, 0)); }, "content\\textures\\widen.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(8.25 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(0, 1)); }, "content\\textures\\heighten.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(6.45 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(-1, 0)); }, "content\\textures\\narrow.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(8.25 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(0, -1)); }, "content\\textures\\shorten.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });

		//Create property value buttons:
		std::vector<std::string> button_texts = {
			"content\\textures\\1.png",
			"content\\textures\\2.png",
			"content\\textures\\3.png",
			"content\\textures\\4.png",
			"content\\textures\\5.png",
		};
		for (auto i = 0; i < 5; i++)
		{
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f((11.7 + 1.75 * i) * BLOCK_SIZE, -2.125 * BLOCK_SIZE), narrow_size, [&, i]() { level.editor.Do<SetPropertyValueOfSelected>(level, i, std::nullopt); }, button_texts[i], default_text_size, [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });
		}

		//Create rotate-buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<std::string> icon_paths = { "content\\textures\\rotate_left.png", "content\\textures\\rotate_right.png" };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(22.25 * BLOCK_SIZE + 1.75 * i * BLOCK_SIZE, row_one), narrow_size, [&, i]() { level.editor.Do<RotateSelectedFields>(level, PI / 2 * (-1 + 2 * i)); }, icon_paths[i], default_text_size, [&]() { return (level.GetEntitiesWith<Selected, ElectricField>().size() != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });
		}

		//Create invert charge button:

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(23.125 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, std::nullopt, -1); }, "content\\textures\\flip.png", default_text_size, [&]() { return (level.GetEntitiesWith<Selected, MagneticField>().size() != 0 || level.GetEntitiesWith<Selected, Charge>().size() != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });

		//Create level size-buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<int> increment = { 1, -1 };
			std::vector<std::string> icon_paths = { "content\\textures\\increase_size.png", "content\\textures\\decrease_size.png" };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(27.4 * BLOCK_SIZE, -UI_BAR_HEIGHT / 2 - 0.875 * BLOCK_SIZE * increment[i]), narrow_size, [&, increment, i]() { level.editor.Do<ModifyLevelSize>(level, increment[i]); }, icon_paths[i], default_text_size, [&, increment, i]() { return (level.GetValidNewSizeId(increment[i]) != 0) && (level.GetEntitiesWithComponent<HelpMenuItem>().size() == 0); });
		}

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(30.75 * BLOCK_SIZE, row_two), narrow_size, [&]() { MusicMenu().Toggle(level); }, "content\\textures\\music.png", default_text_size, []() { return true; });

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(30.75 * BLOCK_SIZE, row_one), narrow_size, [&]() { HelpMenu().Toggle(level); }, "content\\textures\\help.png", default_text_size, []() { return true; });
	}
}
