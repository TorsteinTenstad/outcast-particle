#include "components/blueprint_menu_item.hpp"
#include "components/button.hpp"
#include "components/draw_info.hpp"
#include "components/edit_mode.hpp"
#include "components/physics.hpp"
#include "components/size.hpp"
#include "components/sticky_button.hpp"
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

void EditModeUISystem::Update(Level& level, float dt)
{
	if (level.GetMode() != EDIT_MODE)
	{
		level.DeleteEntitiesWith<EditModeUIEntity>();
		return;
	}
	EditModeUI* ui = level.GetSingleton<EditModeUI>();

	if (!ui->initialized)
	{
		SetupUI(level, ui, dt);
		ui->initialized = true;
	}
	UpdateUI(level, ui);
}

static void UpdateUI(Level& level, EditModeUI* ui)
{
	PlayerMenu().Update(level);
	MusicMenu().Update(level);
	BlueprintMenu().Update(level);
}

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
		{
			auto [entity, _] = CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(1.25 * BLOCK_SIZE, row_one), narrow_size, []() {}, (TEXTURES_DIR / "add.png").string(), "Toggle menu for adding elements", []() { return true; }, globals.key_config.OPEN_BLUEPRINT_MENU);
			level.AddComponent<StickyButton>(entity[0]);
			level.AddComponent<ToggleBlueprintMenuButton>(entity[0]);
		}
		// Delete-button:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(3 * BLOCK_SIZE, row_one), narrow_size, [&level]() { level.editor.Do<DeleteSelected>(level); }, (TEXTURES_DIR / "delete_entity.png").string(), "Delete selected elements", [&level]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); }, globals.key_config.DELETE_ENTITY);

		// Undo- and redo-buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(1.25 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Undo(); }, (TEXTURES_DIR / "undo.png").string(), "Undo", [&level]() { return !level.editor.IsEmpty(); }, globals.key_config.UNDO, true);
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(3. * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Redo(); }, (TEXTURES_DIR / "redo.png").string(), "Redo", [&level]() { return !level.editor.IsAtEnd(); }, globals.key_config.REDO, true);

		// Level size-buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<int> increment = { 1, -1 };
			std::vector<std::string> icon_paths = { (TEXTURES_DIR / "increase_size.png").string(), (TEXTURES_DIR / "decrease_size.png").string() };
			std::vector<std::string> tooltips = { "Increase level size", "Decrease level size" };
			std::vector<sf::Keyboard::Key> shortcut_keys = { globals.key_config.INCREASE_LEVEL_SIZE, globals.key_config.DECREASE_LEVEL_SIZE };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(5.5 * BLOCK_SIZE, -UI_BAR_HEIGHT / 2 - 0.875 * BLOCK_SIZE * increment[i]), narrow_size, [&, increment, i]() { level.editor.Do<ModifyLevelSize>(level, increment[i]); }, icon_paths[i], tooltips[i], [&, increment, i]() { return (level.GetValidNewSizeId(increment[i]) != 0); }, shortcut_keys[i]);
		}

		// Resize buttons:
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(8 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(1, 0)); }, (TEXTURES_DIR / "widen.png").string(), "Make selected elements wider", [&level]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0); }, globals.key_config.INCREMENT_WIDTH);
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(9.75 * BLOCK_SIZE, row_one), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(0, 1)); }, (TEXTURES_DIR / "heighten.png").string(), "Make selected elements taller", [&level]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0); }, globals.key_config.INCREMENT_HEIGHT);
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(8 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(-1, 0)); }, (TEXTURES_DIR / "narrow.png").string(), "Make selected elements thinner", [&level]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0); }, globals.key_config.DECREMENT_WIDTH);
		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(9.75 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<ResizeSelected>(level, sf::Vector2f(0, -1)); }, (TEXTURES_DIR / "shorten.png").string(), "Make selected elements shorter", [&level]() { return (level.GetEntitiesWith<Selected, WidthAndHeight>().size() != 0); }, globals.key_config.DECREMENT_HEIGHT);

		// Property value buttons:
		std::vector<std::string> button_texts = {
			(TEXTURES_DIR / "1.png").string(),
			(TEXTURES_DIR / "2.png").string(),
			(TEXTURES_DIR / "3.png").string(),
			(TEXTURES_DIR / "4.png").string(),
			(TEXTURES_DIR / "5.png").string(),
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
				level, sf::Vector2f((12.5 + 1.75 * i) * BLOCK_SIZE, -2.125 * BLOCK_SIZE), narrow_size, [&, i]() { level.editor.Do<SetPropertyValueOfSelected>(level, i, std::nullopt); }, button_texts[i], "TODO", [&level]() { return (level.GetEntitiesWithComponent<Selected>().size() != 0); }, shortcut_keys[i]);
		}

		// Velocity buttons:
		CreateCanDisableOnPressedButtonWithIcon(
			level, sf::Vector2f(21.75 * BLOCK_SIZE, row_one), narrow_size, [&level, dt]() { level.editor.Do<ChangeVelocityOfSelected>(level, -300.f * dt, 0.f); }, (TEXTURES_DIR / "decrease_velocity.png").string(), "Decrease velocity", [&level]() { return (level.GetEntitiesWith<Selected, Velocity>().size() != 0); }, globals.key_config.DECREMENT_VELOCITY);
		CreateCanDisableOnPressedButtonWithIcon(
			level, sf::Vector2f(23.5 * BLOCK_SIZE, row_one), narrow_size, [&level, dt]() { level.editor.Do<ChangeVelocityOfSelected>(level, 300.f * dt, 0.f); }, (TEXTURES_DIR / "increase_velocity.png").string(), "Increase velocity", [&level]() { return (level.GetEntitiesWith<Selected, Velocity>().size() != 0); }, globals.key_config.INCREMENT_VELOCITY);
		CreateCanDisableOnPressedButtonWithIcon(
			level, sf::Vector2f(21.75 * BLOCK_SIZE, row_two), narrow_size, [&level, dt]() { level.editor.Do<ChangeVelocityOfSelected>(level, 0.f, -dt); }, (TEXTURES_DIR / "rotate_left.png").string(), "Rotate velocity vector clockwise", [&level]() { return (level.GetEntitiesWith<Selected, Velocity>().size() != 0); }, globals.key_config.DECREMENT_VELOCITY_ANGLE);
		CreateCanDisableOnPressedButtonWithIcon(
			level, sf::Vector2f(23.5 * BLOCK_SIZE, row_two), narrow_size, [&level, dt]() { level.editor.Do<ChangeVelocityOfSelected>(level, 0.f, +dt); }, (TEXTURES_DIR / "rotate_right.png").string(), "Rotate velocity vector counter-clockwise", [&level]() { return (level.GetEntitiesWith<Selected, Velocity>().size() != 0); }, globals.key_config.INCREMENT_VELOCITY_ANGLE);

		// Rotate buttons:
		for (int i = 0; i < 2; i++)
		{
			std::vector<std::string> icon_paths = { (TEXTURES_DIR / "rotate_90.png").string(), (TEXTURES_DIR / "counter_rotate_90.png").string() };
			std::vector<sf::Keyboard::Key> shortcut_keys = { globals.key_config.ROTATE_ENTITY_CLOCKWISE, globals.key_config.ROTATE_ENTITY_COUNTER_CLOCKWISE };
			CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(26.25 * BLOCK_SIZE + 1.75 * i * BLOCK_SIZE, row_one), narrow_size, [&, i]() { level.editor.Do<RotateSelectedFields>(level, PI / 2 * (1 - 2 * i)); }, icon_paths[i], "Rotate electronic fields", [&level]() { return (level.GetEntitiesWith<Selected, ElectricField>().size() != 0); }, shortcut_keys[i]);
		}

		// Invert charge button:

		CreateCanDisableButtonWithIcon(
			level, sf::Vector2f(27.125 * BLOCK_SIZE, row_two), narrow_size, [&]() { level.editor.Do<SetPropertyValueOfSelected>(level, std::nullopt, -1); }, (TEXTURES_DIR / "flip.png").string(), "Flip direction of magnetic field and charge of particles", [&level]() { return (level.GetEntitiesWith<Selected, MagneticField>().size() != 0 || level.GetEntitiesWith<Selected, Charge>().size() != 0); }, globals.key_config.EDIT_MODE_FLIP_CHARGES_AND_FIELDS);

		// Help menu button and music button:
		{
			auto [entity, _] = CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(30.75 * BLOCK_SIZE, row_two), narrow_size, []() {}, (TEXTURES_DIR / "music.png").string(), "Toggle music menu", []() { return true; }, globals.key_config.OPEN_MUSIC_MENU);
			level.AddComponent<StickyButton>(entity[0])->channel = 2;
			level.AddComponent<ToggleMusicMenuButton>(entity[0]);
		}
		{
			auto [entity, _] = CreateCanDisableButtonWithIcon(
				level, sf::Vector2f(30.75 * BLOCK_SIZE, row_one), narrow_size, []() {}, (TEXTURES_DIR / "player_icon.png").string(), "Toggle player options menu", []() { return true; }, globals.key_config.OPEN_PLAYER_MENU);
			level.AddComponent<StickyButton>(entity[0])->channel = 2;
			level.AddComponent<TogglePlayerMenuButton>(entity[0]);
		}
	}
}