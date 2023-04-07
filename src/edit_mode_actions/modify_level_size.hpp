#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class ModifyLevelSize : public UndoableAction
{
private:
	Level& level_;
	int increment_;

public:
	ModifyLevelSize(Level& level, int increment) :
		level_(level),
		increment_(increment)
	{}
	void Do()
	{
		level_.ModifyLevelSize(increment_);
	}
	void Undo()
	{
		level_.ModifyLevelSize(-1 * increment_);
	}
};