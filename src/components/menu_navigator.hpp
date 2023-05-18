#pragma once
#include "SFML/Window/Keyboard.hpp"
#include "ecs/entity.hpp"
#include <optional>
#include <vector>

class MenuNavigator
{
public:
	std::optional<Entity> currently_at_entity;
	bool moved_itself_this_frame = false;
	std::optional<std::vector<Entity>> menu_items; //If nullopt: Search for entities with MenuNavigable component
	sf::Keyboard::Key increment_key = sf::Keyboard::Down;
	sf::Keyboard::Key decrement_key = sf::Keyboard::Up;
};

class MenuNavigable
{
};