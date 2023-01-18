#pragma once

#include "game_system.hpp"
#include "level.hpp"

class NamedSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
	}
	
};