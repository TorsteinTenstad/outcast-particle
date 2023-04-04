#pragma once
#include "game_system.hpp"

class EditModeSystem : public GameSystem
{
private:
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
};