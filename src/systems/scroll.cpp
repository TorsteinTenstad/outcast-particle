#include "components/scroll.hpp"
#include "components/button_events.hpp"
#include "components/menu_navigator.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "entity_creation.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include <algorithm>
#include <functional>

std::optional<float> GetNavigatorRequest(Level& level, ScrollWindow* scroll_window, float top, float bottom)
{
	if (!scroll_window->menu_navigator.has_value()) { return std::nullopt; }
	Entity menu_navigator = scroll_window->menu_navigator.value();
	if (!level.IdExists(menu_navigator)) { return std::nullopt; } //Menu navigators may be inactive
	if (level.GetComponent<MenuNavigator>(menu_navigator)->moved_itself_this_frame)
	{
		sf::Vector2f menu_navigator_position = level.GetComponent<Position>(scroll_window->menu_navigator.value())->position;
		float distance_above_top = (top + scroll_window->entity_height / 2) - menu_navigator_position.y;
		if (distance_above_top > 0)
		{
			return distance_above_top;
		}
		float distance_below_bottom = menu_navigator_position.y - (bottom - scroll_window->entity_height / 2);
		if (distance_below_bottom > 0)
		{
			return -distance_below_bottom;
		}
	}
	return std::nullopt;
}
std::optional<float> GetScrollBarRequest(Level& level, const CursorAndKeys& cursor_and_keys, std::optional<Entity> scroll_bar, float view_h, float total_h)
{
	if (!scroll_bar.has_value()
		|| !level.HasComponents<Pressed>(scroll_bar.value())
		|| !cursor_and_keys.cursor_moved_this_frame)
	{
		return std::nullopt;
	}
	float bar_h = level.GetComponent<WidthAndHeight>(scroll_bar.value())->width_and_height.x;
	float bar_max_travel = view_h - bar_h;
	return total_h * (cursor_and_keys.last_frame_cursor_position.y - cursor_and_keys.cursor_position.y) / bar_max_travel;
}

std::optional<Entity> GetScrollBarEntity(Level& level, Entity entity, ScrollWindow* scroll_window, WidthAndHeight* width_and_height, Position* scroll_window_position, float total_h)
{
	float view_w = width_and_height->width_and_height.x;
	float view_h = width_and_height->width_and_height.y;
	if (total_h < view_h)
	{
		level.DeleteChildEntitiesOwnedByComponent<ScrollWindow>(entity);
		return std::nullopt;
	}
	float scroll_bar_w = 0.5 * float(BLOCK_SIZE);
	Entity scroll_bar = GetSingletonChildId<ScrollWindow>(level, entity, [view_w, scroll_bar_w, window_x = scroll_window_position->position.x](Level& level) {
		auto [scroll_bar, _] = CreateMouseEventButton(level, sf::Vector2f(window_x + view_w / 2 + scroll_bar_w, 0), sf::Vector2f(scroll_bar_w, 0));
		return scroll_bar;
	});

	return scroll_bar;
}

void UpdateScrollBar(Level& level, std::optional<Entity> scroll_bar, ScrollWindow* scroll_window, WidthAndHeight* width_and_height, Position* scroll_window_position, float total_h, float max_scroll_distance)
{
	float view_h = width_and_height->width_and_height.y;
	if (!scroll_bar.has_value()) { return; }
	Entity entity = scroll_bar.value();

	float scroll_bar_h = view_h * view_h / total_h;
	float relative_y_travel = scroll_window->scrolled_distance / max_scroll_distance;
	float scroll_bar_position_y = scroll_window_position->position.y
								  - view_h / 2 + scroll_bar_h / 2 + relative_y_travel * (view_h - scroll_bar_h);

	level.GetComponent<WidthAndHeight>(entity)->width_and_height.y = scroll_bar_h;
	level.GetComponent<Position>(entity)->position.y = scroll_bar_position_y;
}

void UpdateScrollingEntities(Level& level, ScrollWindow* scroll_window, float movement, float top, float bottom)
{
	for (auto entity : scroll_window->entities)
	{
		auto [shader, position] = level.GetComponents<Shader, Position>(entity);
		position->position.y += movement;
		shader->float_uniforms["top"] = top;
		shader->float_uniforms["bottom"] = bottom;
	}
	if (!scroll_window->menu_navigator.has_value()) { return; }
	Entity menu_navigator = scroll_window->menu_navigator.value();
	if (!level.IdExists(menu_navigator)) { return; } //Menu navigators may be inactive
	Shader* shader = level.EnsureExistenceOfComponent<Shader>(menu_navigator);
	shader->fragment_shader_path = "SHADERS_DIR\\scroll.frag";
	shader->float_uniforms["top"] = top;
	shader->float_uniforms["bottom"] = bottom;
}

void ScrollSystem::Update(Level& level, float dt)
{
	for (auto [entity, scroll_window, width_and_height, scroll_window_position] : level.GetEntitiesWith<ScrollWindow, WidthAndHeight, Position>())
	{
		if (scroll_window->entities.size() == 0)
		{
			continue;
		}
		float view_w = width_and_height->width_and_height.x;
		float view_h = width_and_height->width_and_height.y;
		float top = scroll_window_position->position.y - view_h / 2;
		float bottom = scroll_window_position->position.y + view_h / 2;
		float total_h = scroll_window->entity_height + scroll_window->bottom_margin
						+ (level.GetComponent<Position>(scroll_window->entities.back())->position.y
							- level.GetComponent<Position>(scroll_window->entities[0])->position.y);
		float max_scroll_distance = std::min(0.f, -(total_h - view_h));
		float old_scrolled_distance = scroll_window->scrolled_distance;

		std::optional<Entity> scroll_bar = GetScrollBarEntity(level, entity, scroll_window, width_and_height, scroll_window_position, total_h);
		float requested_movement = cursor_and_keys_.mouse_wheel_delta * 180;
		requested_movement = GetScrollBarRequest(level, cursor_and_keys_, scroll_bar, view_h, total_h).value_or(requested_movement);
		requested_movement = GetNavigatorRequest(level, scroll_window, top, bottom).value_or(requested_movement);

		float requested_scrolled_distance = scroll_window->scrolled_distance + requested_movement;
		scroll_window->scrolled_distance = Clamp(requested_scrolled_distance, max_scroll_distance, 0.f);
		float movement = scroll_window->scrolled_distance - old_scrolled_distance;

		UpdateScrollBar(level, scroll_bar, scroll_window, width_and_height, scroll_window_position, total_h, max_scroll_distance);
		UpdateScrollingEntities(level, scroll_window, movement, top, bottom);
	}
}