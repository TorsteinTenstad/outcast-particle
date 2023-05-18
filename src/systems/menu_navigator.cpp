#include "components/menu_navigator.hpp"
#include "components/button_events.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/container_operations.hpp"
#include <cassert>

static SnapPosition GetNextSnapPosition(Level& level, const MenuNavigator* menu_navigator, std::vector<SnapPosition> possible_positions, std::function<bool(const SnapPosition&, const SnapPosition&)> ordering_comparison)
{
	assert(possible_positions.size() > 0);
	assert(menu_navigator->current_snap_position.has_value());
	std::sort(possible_positions.begin(), possible_positions.end(), ordering_comparison);
	auto found_it = std::find_if(possible_positions.begin(), possible_positions.end(), [&](SnapPosition x) { return ordering_comparison(menu_navigator->current_snap_position.value(), x); });
	if (found_it == possible_positions.end()) { found_it = possible_positions.begin(); }
	return *found_it;
}

static SnapPosition GetNextSnapPositionBelow(Level& level, const MenuNavigator* menu_navigator, std::vector<SnapPosition> possible_positions)
{
	return GetNextSnapPosition(level, menu_navigator, possible_positions, [](auto a, auto b) { return a.ordering < b.ordering; });
}
static SnapPosition GetNextSnapPositionAbove(Level& level, const MenuNavigator* menu_navigator, std::vector<SnapPosition> possible_positions)
{
	return GetNextSnapPosition(level, menu_navigator, possible_positions, [](auto a, auto b) { return a.ordering > b.ordering; });
}

template <class View>
static Entity DropComponentsFromView(View view)
{
	return std::get<Entity>(view);
}

template <class View>
static std::vector<Entity> DropComponentsFromViews(std::vector<View> views)
{
	std::function<Entity(View)> f = DropComponentsFromView<View>;
	return ApplyFuncToVector(views, f);
}

static std::vector<SnapPosition> GetPossiblePossiblePositions(Level& level, MenuNavigator* menu_navigator)
{
	std::vector<SnapPosition> possible_positions;
	std::vector<Entity> menu_items = menu_navigator->menu_items.value_or(DropComponentsFromViews(level.GetEntitiesWith<MenuNavigable, WidthAndHeight, Position>()));
	for (Entity entity : menu_items)
	{
		possible_positions.emplace_back(level, entity);
	}
	return possible_positions;
}

template <class... Components>
std::tuple<Entity, Components*...> GetHighestDrawPriority(std::vector<std::tuple<Entity, Components*...>> entity_views)
{
	return *std::max_element(entity_views.begin(), entity_views.end(), [](const std::tuple<Entity, Components*...>& a, const std::tuple<Entity, Components*...>& b) {
		return std::get<DrawPriority*>(a)->draw_priority < std::get<DrawPriority*>(b)->draw_priority;
	});
}

static void MoveToSnapPosition(Level& level, Entity entity, MenuNavigator* menu_navigator, const SnapPosition& snap_position)
{
	menu_navigator->current_snap_position = snap_position;
	menu_navigator->moved_itself_this_frame = true;
	level.GetComponent<Position>(entity)->position = snap_position.position - sf::Vector2f(level.GetComponent<WidthAndHeight>(entity)->width_and_height.x, 0);
}

void MenuNavigatorSystem::Update(Level& level, float dt)
{
	auto navigators = level.GetEntitiesWith<MenuNavigator, DrawPriority, WidthAndHeight, Position>();
	if (navigators.size() == 0)
	{
		for (Entity entity : level.inactive_entities.GetEntitiesWithComponent<MenuNavigator>())
		{
			level.ActivateEntities(entity);
		}
		navigators = level.GetEntitiesWith<MenuNavigator, DrawPriority, WidthAndHeight, Position>();
	}
	if (navigators.size() == 0) { return; }

	auto highest_priority_navigator = GetHighestDrawPriority(navigators);

	for (auto navigator_view : navigators)
	{
		Entity entity = std::get<Entity>(navigator_view);
		if (entity == std::get<Entity>(highest_priority_navigator)) { continue; }
		level.DeactivateEntities(entity);
	}

	auto [entity, menu_navigator, draw_priority, width_and_height, position] = highest_priority_navigator;
	menu_navigator->moved_itself_this_frame = false;
	std::vector<SnapPosition> possible_positions = GetPossiblePossiblePositions(level, menu_navigator);
	assert(possible_positions.size() > 0);

	if (!menu_navigator->current_snap_position.has_value())
	{
		return MoveToSnapPosition(level, entity, menu_navigator, *std::min_element(possible_positions.begin(), possible_positions.end(), [](auto a, auto b) { return a.ordering < b.ordering; }));
	}
	else if (cursor_and_keys_.key_pressed_this_frame[menu_navigator->increment_key])
	{
		cursor_and_keys_.key_pressed_this_frame[menu_navigator->increment_key] = false;
		MoveToSnapPosition(level, entity, menu_navigator, GetNextSnapPositionBelow(level, menu_navigator, possible_positions));
	}
	else if (cursor_and_keys_.key_pressed_this_frame[menu_navigator->decrement_key])
	{
		cursor_and_keys_.key_pressed_this_frame[menu_navigator->decrement_key] = false;
		MoveToSnapPosition(level, entity, menu_navigator, GetNextSnapPositionAbove(level, menu_navigator, possible_positions));
	}
	else
	{
		for (auto [hovered_started_this_frame_id, hovered_started_this_frame, menu_navigable, width_and_height, position] : level.GetEntitiesWith<HoveredStartedThisFrame, MenuNavigable, WidthAndHeight, Position>())
		{
			MoveToSnapPosition(level, entity, menu_navigator, SnapPosition(level, hovered_started_this_frame_id));
			break;
		}
	}
}