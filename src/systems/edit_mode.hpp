#pragma once
#include "game_system.hpp"
#include "level_editor.hpp"

class EditModeSystem : public GameSystem
{
private:
	LevelEditor level_editor_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
};