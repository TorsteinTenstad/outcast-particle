#pragma once
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class TimerSystem : public GameSystem
{
private:
	const std::string* level_id_;

public:
	using GameSystem::GameSystem;
	void Give(const std::string* level_id);
	void Update(Level& level, float dt);
};
