#pragma once
#include "game_system.hpp"
#include "undo_system.hpp"

class EditModeSystem : public GameSystem
{
private:
	UndoSystem level_editor_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
};