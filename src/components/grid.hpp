#pragma once
#include "constants.hpp"
#include <map>

class Grid
{
public:
	float grid_size = BLOCK_SIZE;
	std::map<int, std::map<int, bool>> grid;
};