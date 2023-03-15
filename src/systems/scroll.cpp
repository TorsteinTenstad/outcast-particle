#include "_pure_DO_systems.hpp"
#include "level.hpp"
#include "utils/math.hpp"
#include <algorithm>

void ScrollSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, scroll_window, width_and_height, scroll_window_position] : level.GetEntitiesWith<ScrollWindow, WidthAndHeight, Position>())
	{
		float top = scroll_window_position->position.y - width_and_height->width_and_height.y / 2;
		float bottom = scroll_window_position->position.y + width_and_height->width_and_height.y / 2;
		float requested_movement = cursor_and_keys_.mouse_wheel_delta * 180;
		if (scroll_window->menu_navigator.has_value())
		{
			sf::Vector2f menu_navigator_position = level.GetComponent<Position>(scroll_window->menu_navigator.value())->position;
			float distance_above_top = (top + scroll_window->entity_height / 2) - menu_navigator_position.y;
			if (distance_above_top > 0)
			{
				requested_movement += distance_above_top;
			}
			float distance_below_bottom = menu_navigator_position.y - (bottom - scroll_window->entity_height / 2);
			if (distance_below_bottom > 0)
			{
				requested_movement -= distance_below_bottom;
			}
		}
		float max_scroll = -(scroll_window->entity_height + scroll_window->bottom_margin
			+ (level.GetComponent<Position>(scroll_window->entities.back())->position.y
				- level.GetComponent<Position>(scroll_window->entities[0])->position.y)
			- width_and_height->width_and_height.y);
		max_scroll = std::min(0.f, max_scroll); //max_scroll is negative
		float old_scolled_distance = scroll_window->scolled_distance;
		float requested_scolled_distance = scroll_window->scolled_distance + requested_movement;
		scroll_window->scolled_distance = Clamp(requested_scolled_distance, max_scroll, 0.f);
		float movement = scroll_window->scolled_distance - old_scolled_distance;
		for (auto entity_id : scroll_window->entities)
		{
			auto [shader, position] = level.GetComponents<Shader, Position>(entity_id);
			position->position.y += movement;
			shader->float_uniforms["top"] = top;
			shader->float_uniforms["bottom"] = bottom;
		}
	}
}