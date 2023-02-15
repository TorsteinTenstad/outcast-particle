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
	std::map<std::array<int, 2>, int> entity_id_at_position;
};