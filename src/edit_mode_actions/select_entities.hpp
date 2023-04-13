#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

static void Deselect(Level& level, std::vector<int> entity_ids)
{
	for (int id : entity_ids)
	{
		level.RemoveComponents<Selected>(id);
	}
}

static void Select(Level& level, std::vector<int> entity_ids)
{
	for (int id : entity_ids)
	{
		level.EnsureExistenceOfComponent<Selected>(id);
	}
}

class SelectEntities : public UndoableAction
{
private:
	Level& level_;
	std::vector<int> entities_to_select_;
	std::vector<int> entities_to_deselect_;

public:
	SelectEntities(Level& level, std::vector<int>&& entities_to_select, std::vector<int>&& entities_to_deselect) :
		level_(level),
		entities_to_select_(entities_to_select),
		entities_to_deselect_(entities_to_deselect)
	{}
	void Do()
	{
		Deselect(level_, entities_to_deselect_);
		Select(level_, entities_to_select_);
	}
	void Undo()
	{
		Deselect(level_, entities_to_select_);
		Select(level_, entities_to_deselect_);
	}
};