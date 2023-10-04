#pragma once
#include "edit_mode.hpp"
#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/editable.hpp"
#include "components/intersection.hpp"
#include "components/not_serialized.hpp"
#include "components/physics.hpp"
#include "components/position.hpp"
#include "components/scheduled_delete.hpp"
#include "components/size.hpp"
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
#include "edit_mode_actions/set_property_value_of_selected.hpp"
#include "edit_mode_menus.hpp"
#include "globals.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/check_intersection.hpp"
#include "utils/container_operations.hpp"
#include "utils/get_size.hpp"
#include "utils/level_id.hpp"
#include "utils/math.hpp"
#include <optional>

const float DEFAULT_VELOCITY_MAGNITUDE_CHANGE_SENSITIVITY = 400;
const float DEFAULT_VELOCITY_ANGLE_CHANGE_SENSITIVITY = PI / 2;

static void ShowCopyPreview(Level& level, sf::Vector2f origin)
{
	for (auto [entity, selected, position] : level.GetEntitiesWith<Selected, Position>())
	{
		if (level.HasComponents<Player>(entity)) { continue; }
		Entity copy = level.CopyEntity(entity);
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
	if (level.GetEntitiesWithComponent<Pressed>().size() > 0)
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
		if (level_mode != PAUSE_MODE && level.GetEntitiesWithComponent<Selected>().size() > 0)
		{
			level.editor.Do<SelectEntities>(level, std::vector<Entity> {}, level.GetEntitiesWithComponent<Selected>());
		}
		return;
	}

	if (std::optional<Entity> selected_entity = BlueprintMenu().HandleSelection(level))
	{
		Entity entity = selected_entity.value();
		level.editor.Do<AddEntity>(level, selected_entity.value());
	}

	Tool current_tool = ComputeCurrentTool(level, cursor_and_keys_);

	// Rectangle select
	if (current_tool == Selecting)
	{
		std::function<Entity(ECSScene&)> creation_func = [](ECSScene& scene) { return std::get<0>(scene.CreateEntityWith<Intersection, WidthAndHeight, Position>()); };
		Entity rectangle_select_tool_id = level.GetSingletonId<EditModeRectangleSelectTool>(creation_func);
		WidthAndHeight* tool_width_and_height = level.GetComponent<WidthAndHeight>(rectangle_select_tool_id);
		Position* tool_position = level.GetComponent<Position>(rectangle_select_tool_id);
		sf::Vector2f size = cursor_and_keys_.mouse_button_last_pressed_position[sf::Mouse::Left] - cursor_and_keys_.cursor_position;
		tool_position->position = cursor_and_keys_.cursor_position + size / 2.f;
		size = Abs(size);
		tool_width_and_height->width_and_height = size;

		for (auto [entity, editable, position] : level.GetEntitiesWith<Editable, Position>())
		{
			WidthAndHeight* width_and_height = level.RawGetComponent<WidthAndHeight>(entity);
			Radius* radius = level.RawGetComponent<Radius>(entity);
			bool intersects_rectangle_tool = ((width_and_height && CheckIntersection(tool_position, position, tool_width_and_height, width_and_height))
											  || (radius && CheckIntersection(tool_position, position, tool_width_and_height, radius)));
			bool is_selected = level.HasComponents<Selected>(entity);

			if (is_selected && !intersects_rectangle_tool)
			{
				level.editor.Do<SelectEntities>(level, std::vector<Entity>({}), std::vector<Entity>({ entity }));
				continue;
			}
			if (!is_selected && intersects_rectangle_tool)
			{
				level.editor.Do<SelectEntities>(level, std::vector<Entity>({ entity }), std::vector<Entity>({}));
				continue;
			}
		}
	}
	else
	{
		level.DeleteEntity(level.FindSingletonId<EditModeRectangleSelectTool>());
	}

	// Select entities on click:
	for (auto [entity, pressed_this_frame, editable] : level.GetEntitiesWith<PressedThisFrame, Editable>())
	{
		if (!level.HasComponents<Selected>(entity))
		{
			bool deselect_others = !cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES];
			level.editor.Do<SelectEntities>(level, std::vector<Entity>({ entity }), deselect_others ? level.GetEntitiesWithComponent<Selected>() : std::vector<Entity> {});
		}
		else if (cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES])
		{
			level.editor.Do<SelectEntities>(level, std::vector<Entity> {}, std::vector<Entity>({ entity }));
		}
	}

	// Move entities with the curser:
	if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left])
	{
		for (auto [entity, selected, position] : level.GetEntitiesWith<Selected, Position>())
		{
			selected->mouse_offset = position->position - cursor_and_keys_.cursor_position;
		}
	}
	if (current_tool == Moving
		&& cursor_and_keys_.cursor_moved_this_frame
		&& level.GetEntitiesWithComponent<Selected>().size() != 0
		&& level.GetEntitiesWith<Pressed, OnReleasedThisFrame>().size() == 0)
	{
		level.editor.Do<MoveSelectedWithCursor>(level, cursor_and_keys_.cursor_position);
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
}