#include "components/blueprint_menu_item.hpp"
#include "components/button.hpp"
#include "components/draw_info.hpp"
#include "components/edit_mode.hpp"
#include "components/physics.hpp"
#include "components/size.hpp"
#include "components/text.hpp"
#include "edit_mode_actions/change_velocity_of_selected.hpp"
#include "edit_mode_actions/copy_selected.hpp"
#include "edit_mode_actions/delete_selected.hpp"
#include "edit_mode_actions/modify_level_size.hpp"
#include "edit_mode_actions/resize_selected.hpp"
#include "edit_mode_actions/rotate_selected_fields.hpp"
#include "edit_mode_actions/set_property_value_of_selected.hpp"
#include "edit_mode_menus.hpp"
#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "systems/edit_mode_menus.hpp"
#include "utils/string_parsing.hpp"

static void UpdateUI(Level& level, EditModeUI* ui);
static void SetupUI(Level& level, EditModeUI* ui, float dt);

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
		SetupUI(level, ui, dt);
		ui->initialized = true;
	}
	UpdateUI(level, ui);
}

static void UpdateUI(Level& level, EditModeUI* ui)
{}

static void SetupUI(Level& level, EditModeUI* ui, float dt)
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

		// Create add-entity-button:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(1.25 * BLOCK_SIZE, row_one), narrow_size, [&level]() { BlueprintMenu().Toggle(level); }, "content\\textures\\add.png", "Open/close menu for adding elements", []() { return true; }, globals.key_config.OPEN_BLUEPRINT_MENU);

		// Delete-button:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(3 * BLOCK_SIZE, row_one), narrow_size, [&level]() { level.editor.Do<DeleteSelected>(level); }, "content\\textures\\delete_entity.png", "Delete selected elements", [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.DELETE_ENTITY);

		// Undo- and redo-buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(1.25 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Undo(); }, "content\\textures\\undo.png", "Undo", [&]() { return !level.editor.IsEmpty() && (!HelpMenu().IsOpen(level)); }, globals.key_config.UNDO, true);
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(3. * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Redo(); }, "content\\textures\\redo.png", "Redo", [&]() { return !level.editor.IsAtEnd() && (!HelpMenu().IsOpen(level)); }, globals.key_config.REDO, true);

		// Level size-buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<int> increment = { 1, -1 };
			std::vector<std::string> icon_paths = { "content\\textures\\increase_size.png", "content\\textures\\decrease_size.png" };
			std::vector<std::string> tooltips = { "Increase level size", "Decrease level size" };
			std::vector<sf::Keyboard::Key> shortcut_keys = { globals.key_config.INCREASE_LEVEL_SIZE, globals.key_config.DECREASE_LEVEL_SIZE };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(5.5 * BLOCK_SIZE, -UI_BAR_HEIGHT / 2 - 0.875 * BLOCK_SIZE * increment[i]), narrow_size, [&, increment, i]() { level.editor.Do<ModifyLevelSize>(level, increment[i]); }, icon_paths[i], tooltips[i], [&, increment, i]() { return (level.GetValidNewSizeId(increment[i]) != 0) && (!HelpMenu().IsOpen(level)); }, shortcut_keys[i]);
		}

		// Resize buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(8 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(1, 0)); }, "content\\textures\\widen.png", "Make selected elements wider", [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.INCREMENT_WIDTH);
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(9.75 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(0, 1)); }, "content\\textures\\heighten.png", "Make selected elements taller", [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.INCREMENT_HEIGHT);
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(8 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(-1, 0)); }, "content\\textures\\narrow.png", "Make selected elements thinner", [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.DECREMENT_WIDTH);
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(9.75 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(0, -1)); }, "content\\textures\\shorten.png", "Make selected elements shorter", [&]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.DECREMENT_WIDTH);

		// Property value buttons:
		std::vector<std::string> button_texts = {
			"content\\textures\\1.png",
			"content\\textures\\2.png",
			"content\\textures\\3.png",
			"content\\textures\\4.png",
			"content\\textures\\5.png",
		};
		std::vector<sf::Keyboard::Key> shortcut_keys = {
			sf::Keyboard::Num1,
			sf::Keyboard::Num2,
			sf::Keyboard::Num3,
			sf::Keyboard::Num4,
			sf::Keyboard::Num5,
		};
		for (auto i = 0; i < 5; i++)
		{
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f((12.5 + 1.75 * i) * BLOCK_SIZE, -2.125 * BLOCK_SIZE), narrow_size, [&, i]() { level.editor.Do<SetPropertyValueOfSelected>(level, i, std::nullopt); }, button_texts[i], "TODO", [&]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0) && (!HelpMenu().IsOpen(level)); }, shortcut_keys[i]);
		}

		// Velocity buttons:
		CreateCanDisableOnPressButtonWithIcon(
			level, sf::Vector2f(21.75 * BLOCK_SIZE, row_one), narrow_size, [&, dt]() { level.editor.Do<ChangeVelocityOfSelected>(level, -300 * dt, 0); }, "content\\textures\\decrease_velocity.png", "Decrease velocity", [&]() { return (level.GetEntitiesWith<Selected, Velocity>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.DECREMENT_VELOCITY);
		CreateCanDisableOnPressButtonWithIcon(
			level, sf::Vector2f(23.5 * BLOCK_SIZE, row_one), narrow_size, [&, dt]() { level.editor.Do<ChangeVelocityOfSelected>(level, 300 * dt, 0); }, "content\\textures\\increase_velocity.png", "Increase velocity", [&]() { return (level.GetEntitiesWith<Selected, Velocity>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.INCREMENT_VELOCITY);
		CreateCanDisableOnPressButtonWithIcon(
			level, sf::Vector2f(21.75 * BLOCK_SIZE, row_two), narrow_size, [&, dt]() { level.editor.Do<ChangeVelocityOfSelected>(level, 0, -dt); }, "content\\textures\\rotate_left.png", "Rotate velocity vector clockwise", [&]() { return (level.GetEntitiesWith<Selected, Velocity>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.DECREMENT_VELOCITY_ANGLE);
		CreateCanDisableOnPressButtonWithIcon(
			level, sf::Vector2f(23.5 * BLOCK_SIZE, row_two), narrow_size, [&, dt]() { level.editor.Do<ChangeVelocityOfSelected>(level, 0, +dt); }, "content\\textures\\rotate_right.png", "Rotate velocity vector counter-clockwise", [&]() { return (level.GetEntitiesWith<Selected, Velocity>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.INCREMENT_VELOCITY_ANGLE);

		// Rotate buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<std::string> icon_paths = { "content\\textures\\rotate_90.png", "content\\textures\\counter_rotate_90.png" };
			std::vector<sf::Keyboard::Key> shortcut_keys = { globals.key_config.ROTATE_ENTITY_CLOCKWISE, globals.key_config.ROTATE_ENTITY_COUNTER_CLOCKWISE };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(26.25 * BLOCK_SIZE + 1.75 * i * BLOCK_SIZE, row_one), narrow_size, [&, i]() { level.editor.Do<RotateSelectedFields>(level, PI / 2 * (1 - 2 * i)); }, icon_paths[i], "Rotate electronic fields", [&]() { return (level.GetEntitiesWith<Selected, ElectricField>().size() != 0) && (!HelpMenu().IsOpen(level)); }, shortcut_keys[i]);
		}

		// Invert charge button:

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(27.125 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, std::nullopt, -1); }, "content\\textures\\flip.png", "Flip direction of magnetic field and charge of particles", [&]() { return (level.GetEntitiesWith<Selected, MagneticField>().size() != 0 || level.GetEntitiesWith<Selected, Charge>().size() != 0) && (!HelpMenu().IsOpen(level)); }, globals.key_config.EDIT_MODE_FLIP_CHARGES_AND_FIELDS);

		// Help menu button and music button:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(30.75 * BLOCK_SIZE, row_two), narrow_size, [&]() { MusicMenu().Toggle(level); }, "content\\textures\\music.png", "Open music menu", []() { return true; }, globals.key_config.OPEN_MUSIC_MENU);

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(30.75 * BLOCK_SIZE, row_one), narrow_size, [&]() { HelpMenu().Toggle(level); }, "content\\textures\\help.png", "Show help screen", []() { return true; }, globals.key_config.OPEN_HELP_MENU);
	}
}
