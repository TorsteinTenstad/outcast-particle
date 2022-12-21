#pragma once
#include "PCH.hpp"
#include "game_system.hpp"
#include "level.hpp"

class NamedSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
	}
	void OnEnterMode(Level& level, Mode mode) {};
	void OnExitMode(Level& level, Mode mode) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};