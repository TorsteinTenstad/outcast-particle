#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "ecs/entity.hpp"
#include "level.hpp"
#include <optional>
#include <vector>

struct SnapPosition
{
	Entity entity;
	sf::Vector2f position;
	float ordering;

	SnapPosition(Level& level, Entity entity) :
		entity(entity)
	{
		auto [width_and_height, position_component] = level.GetComponents<WidthAndHeight, Position>(entity);
		position = position_component->position;
		position.x -= width_and_height->width_and_height.x / 2;
		ordering = position.x + position.y;
	}
};

class MenuNavigator
{
public:
	std::optional<SnapPosition> current_snap_position;
	bool moved_itself_this_frame = false;
	std::optional<std::vector<Entity>> menu_items; //If nullopt: Search for entities with MenuNavigable component
	sf::Keyboard::Key increment_key = sf::Keyboard::Down;
	sf::Keyboard::Key decrement_key = sf::Keyboard::Up;
};

class MenuNavigable
{
};