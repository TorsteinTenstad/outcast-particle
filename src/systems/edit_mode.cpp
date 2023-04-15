#pragma once
#include "edit_mode.hpp"
#include "components/editable.hpp"
#include "components/physics.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "edit_mode_actions/add_entity.hpp"
#include "edit_mode_actions/copy_selected.hpp"
#include "edit_mode_actions/delete_selected.hpp"
#include "edit_mode_actions/modify_level_size.hpp"
#include "edit_mode_actions/move_selected_with_cursor.hpp"
#include "edit_mode_actions/resize_selected.hpp"
#include "edit_mode_actions/rotate_selected_fields.hpp"
#include "edit_mode_actions/select_entities.hpp"
#include "edit_mode_blueprint_menu_functions.hpp"
#include "globals.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/get_size.hpp"
#include "utils/level_id.hpp"
#include "utils/math.hpp"
#include <optional>

const float DEFAULT_VELOCITY_MAGNITUDE_CHANGE_SENSITIVITY = 400;
const float DEFAULT_VELOCITY_ANGLE_CHANGE_SENSITIVITY = PI / 2;

static void ShowCopyPreview(Level& level, sf::Vector2f origin)
{
	for (auto [entity_id, selected, position] : level.GetEntitiesWith<Selected, Position>())
	{
		int copy = level.CopyEntity(entity_id);
		level.GetComponent<Position>(copy)->position = origin + selected->mouse_offset;
		level.RemoveComponents<Selected>(copy);
		level.EnsureExistenceOfComponent<ScheduledDelete>(copy)->frames_left_to_live = 1;
	}
}

enum Tool
{
	Idle,
	Moving,
	Selecting
};

Tool ComputeCurrentTool(Level& level, CursorAndKeys cursor_and_keys)
{
	if (!cursor_and_keys.mouse_button_down[sf::Mouse::Left])
	{
		return Idle;
	}
	if (level.GetIdsWithComponent<Pressed>().size() > 0)
	{
		return Moving;
	}
	return Selecting;
}

void EditModeSystem::Update(Level& level, float dt)
{
	auto level_mode = level.GetMode();
	if (level_mode != EDIT_MODE)
	{
		if (level_mode != PAUSE_MODE && level.GetIdsWithComponent<Selected>().size() > 0)
		{
			level.editor.Do<SelectEntities>(level, std::vector<int> {}, level.GetIdsWithComponent<Selected>());
		}
		return;
	}

	if (cursor_and_keys_.key_down[sf::Keyboard::LControl] && cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Z])
	{
		level.editor.Undo();
	}
	if (cursor_and_keys_.key_down[sf::Keyboard::LControl] && cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Y])
	{
		level.editor.Redo();
	}

	// Handle selection of entity in blueprint menu:
	if (std::optional<int> selected_entity = UpdateBlueprintMenu(level))
	{
		level.editor.Do<AddEntity>(level, selected_entity.value());
	}

	Tool current_tool = ComputeCurrentTool(level, cursor_and_keys_);

	// Rectangle select
	std::function<int(ECSScene&)> creation_func = [](ECSScene& scene) { return std::get<0>(scene.CreateEntityWith<Intersection, WidthAndHeight, Position>()); };
	int drag_select_tool_id = level.GetSingleton("EditModeDragSelectTool", creation_func);

	if (current_tool == Selecting)
	{
		sf::Vector2f size = cursor_and_keys_.mouse_button_last_pressed_position[sf::Mouse::Left] - cursor_and_keys_.cursor_position;
		sf::Vector2f position = cursor_and_keys_.cursor_position + size / 2.f;
		size = Abs(size);
		level.GetComponent<WidthAndHeight>(drag_select_tool_id)->width_and_height = size;
		level.GetComponent<Position>(drag_select_tool_id)->position = position;
	}
	else
	{
		level.GetComponent<WidthAndHeight>(drag_select_tool_id)->width_and_height = sf::Vector2f(0, 0);
	}

	for (auto entity : level.GetComponent<Intersection>(drag_select_tool_id)->entered_this_frame_ids)
	{
		if (level.HasComponents<Editable>(entity) && !level.HasComponents<Selected>(entity))
		{
			level.editor.Do<SelectEntities>(level, std::vector<int>({ entity }), std::vector<int>({}));
		}
	}
	if (current_tool == Selecting)
	{
		for (auto entity : level.GetComponent<Intersection>(drag_select_tool_id)->left_this_frame_ids)
		{
			if (level.HasComponents<Editable, Selected>(entity))
			{
				level.editor.Do<SelectEntities>(level, std::vector<int>({}), std::vector<int>({ entity }));
			}
		}
	}

	// Select entities on click:
	for (auto [entity_id, pressed_this_frame, editable] : level.GetEntitiesWith<PressedThisFrame, Editable>())
	{
		if (!level.HasComponents<Selected>(entity_id))
		{
			bool deselect_others = !cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES];
			level.editor.Do<SelectEntities>(level, std::vector<int>({ entity_id }), deselect_others ? level.GetIdsWithComponent<Selected>() : std::vector<int> {});
		}
		else if (cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES])
		{
			level.editor.Do<SelectEntities>(level, std::vector<int> {}, std::vector<int>({ entity_id }));
		}
	}

	// Conditional deselect all:
	if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left] && level.GetEntitiesWith<PressedThisFrame, Selected>().size() == 0
		&& !cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY] && !cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES])
	{
		level.editor.Do<SelectEntities>(level, std::vector<int> {}, level.GetIdsWithComponent<Selected>());
	}

	// Move entities with the curser:
	if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left])
	{
		for (auto [entity_id, selected, position] : level.GetEntitiesWith<Selected, Position>())
		{
			selected->mouse_offset = position->position - cursor_and_keys_.cursor_position;
		}
	}
	if (current_tool == Moving && cursor_and_keys_.cursor_moved_this_frame)
	{
		level.editor.Do<MoveSelectedWithCursor>(level, cursor_and_keys_.cursor_position);
	}

	// Resize:
	for (auto [key, size_delta_step] : std::vector<std::tuple<sf::Keyboard::Key, sf::Vector2f>>(
			 { { globals.key_config.INCREMENT_HEIGHT, sf::Vector2f(0, 1) },
				 { globals.key_config.DECREMENT_HEIGHT, sf::Vector2f(0, -1) },
				 { globals.key_config.INCREMENT_WIDTH, sf::Vector2f(1, 0) },
				 { globals.key_config.DECREMENT_WIDTH, sf::Vector2f(-1, 0) } }))
	{
		if (cursor_and_keys_.key_pressed_this_frame[key])
		{
			level.editor.Do<ResizeSelected>(level, size_delta_step);
		}
	}

	// Delete:
	if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DELETE_ENTITY])
	{
		level.editor.Do<DeleteSelected>(level);
	}

	// Change level size:
	for (auto [key, increment] : std::vector<std::tuple<sf::Keyboard::Key, int>>(
			 { { globals.key_config.INCREASE_LEVEL_SIZE, +1 },
				 { globals.key_config.DECREASE_LEVEL_SIZE, -1 } }))
	{
		if (!cursor_and_keys_.key_pressed_this_frame[key])
		{
			continue;
		}
		if (level.GetValidNewSizeId(increment) == 0)
		{
			continue;
		}
		level.editor.Do<ModifyLevelSize>(level, increment);
	}

	// Rotate fields:
	if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.ROTATE_ENTITY])
	{
		level.editor.Do<RotateSelectedFields>(level, PI / 2);
	}

	// Copy selected:
	if (cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY])
	{
		if (cursor_and_keys_.mouse_button_released_this_frame[sf::Mouse::Left])
		{
			level.editor.Do<CopySelected>(level, cursor_and_keys_.mouse_button_last_pressed_position[sf::Mouse::Left]);
		}
		else if (current_tool == Moving)
		{
			ShowCopyPreview(level, cursor_and_keys_.mouse_button_last_pressed_position[sf::Mouse::Left]);
		}
	}

	return;

	// Edit charge:
	for (auto [entity_id, selected, charge] : level.GetEntitiesWith<Selected, Charge>())
	{
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.EDIT_MODE_SWITCH_CHARGE])
		{
			charge->charge *= -1;
		}

		for (unsigned i = 0; i < CHARGE_CATEGORY_KEYS.size(); ++i)
		{
			if (cursor_and_keys_.key_pressed_this_frame[CHARGE_CATEGORY_KEYS[i]])
			{
				charge->charge = Sign(charge->charge) * abs(PARTICLE_CHARGE_CATEGORIES[i]);
			}
		}
	}

	// Edit velocity:
	for (auto [entity_id, selected, velocity] : level.GetEntitiesWith<Selected, Velocity>())
	{
		float velocity_magnitude = Magnitude(velocity->velocity);
		float velocity_angle = Angle(velocity->velocity);

		float sensitivity_modifier = cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1;
		if (cursor_and_keys_.key_down[globals.key_config.INCREMENT_VELOCITY])
		{
			velocity_magnitude += DEFAULT_VELOCITY_MAGNITUDE_CHANGE_SENSITIVITY * dt / sensitivity_modifier;
		}
		if (cursor_and_keys_.key_down[globals.key_config.DECREMENT_VELOCITY])
		{
			velocity_magnitude -= DEFAULT_VELOCITY_MAGNITUDE_CHANGE_SENSITIVITY * dt / sensitivity_modifier;
		}
		if (cursor_and_keys_.key_down[globals.key_config.INCREMENT_VELOCITY_ANGLE])
		{
			velocity_angle += DEFAULT_VELOCITY_ANGLE_CHANGE_SENSITIVITY * dt / sensitivity_modifier;
		}
		if (cursor_and_keys_.key_down[globals.key_config.DECREMENT_VELOCITY_ANGLE])
		{
			velocity_angle -= DEFAULT_VELOCITY_ANGLE_CHANGE_SENSITIVITY * dt / sensitivity_modifier;
		}

		velocity->velocity = Vector2fFromPolar(velocity_magnitude, velocity_angle);
	}
	// Edit magnetic field:
	for (auto [entity_id, selected, magnetic_field] : level.GetEntitiesWith<Selected, MagneticField>())
	{
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.EDIT_MODE_SWITCH_MAGNETIC_FIELD_DIRECTION])
		{
			magnetic_field->field_strength *= -1;
		}
		for (unsigned i = 0; i < MAGNETIC_FIELD_CATEGORY_KEYS.size(); ++i)
		{
			if (cursor_and_keys_.key_pressed_this_frame[MAGNETIC_FIELD_CATEGORY_KEYS[i]])
			{
				magnetic_field->field_strength = Sign(magnetic_field->field_strength) * abs(MAGNETIC_FIELD_STRENGTH_CATEGORIES[i]);
			}
		}
	}

	// Edit electric field:
	for (auto [entity_id, selected, electric_field] : level.GetEntitiesWith<Selected, ElectricField>())
	{
		for (unsigned i = 0; i < ELECTRIC_FIELD_CATEGORY_KEYS.size(); ++i)
		{
			if (cursor_and_keys_.key_pressed_this_frame[ELECTRIC_FIELD_CATEGORY_KEYS[i]])
			{
				electric_field->field_vector = Normalized(electric_field->field_vector) * abs(ELECTRIC_FIELD_STRENGTH_CATEGORIES[i]);
			}
		}
	}
}