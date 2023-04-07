#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class DeleteSelected : public UndoableAction
{
private:
	Level& level_;
	std::vector<int> entities_;

public:
	DeleteSelected(Level& level) :
		level_(level)
	{
		entities_ = level.GetIdsWithComponent<Selected>();
	}
	void Do()
	{
		for (int entity : entities_)
		{
			level_.DeactivateEntity(entity);
		}
	}
	void Undo()
	{
		for (int entity : entities_)
		{
			level_.ActivateEntity(entity);
		}
	}
};