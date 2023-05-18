#pragma once
#include "ecs/entity.hpp"

// Entities with this component die upon intersection with player

class Coin
{
};

class CoinCounter
{
public:
	int coin_counter = 0;
};