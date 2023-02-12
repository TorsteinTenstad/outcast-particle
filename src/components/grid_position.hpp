#pragma once
#include "SFML/System/Vector2.hpp"
#include <map>

class GridPosition
{
public:
	sf::Vector2i grid_position;
};

class GridPositionManager
{
public:
	bool dirty = true;
	std::map<sf::Vector2i, int> entity_id_at_position;
};