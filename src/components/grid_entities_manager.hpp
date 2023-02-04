#pragma once
#include "constants.hpp"
#include <map>

enum GridEntity
{
	EMPTY = 0,
	WALL,
	BOUNCE_WALL,
	NO_BOUNCE_WALL
};

class GridEntitiesManager
{
public:
	float grid_size = BLOCK_SIZE;
	std::map<int, std::map<int, GridEntity>> grid;
};