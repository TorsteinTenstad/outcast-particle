#pragma once
#include "SFML/Window/Keyboard.hpp"
#include <optional>
#include <vector>

class MenuNavigator
{
public:
	std::optional<int> currently_at_entity_id;
	bool moved_itself_this_frame = false;
	std::optional<std::vector<int>> menu_items; //If nullopt: Search for entities with MenuNavigable component
	sf::Keyboard::Key increment_key = sf::Keyboard::Down;
	sf::Keyboard::Key decrement_key = sf::Keyboard::Up;
};

class MenuNavigable
{
};