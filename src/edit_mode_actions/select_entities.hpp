#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

static void Deselect(Level& level, std::vector<Entity> entities)
{
	for (Entity entity : entities)
	{
		level.RemoveComponents<Selected>(entity);
	}
}

static void Select(Level& level, std::vector<Entity> entities)
{
	for (Entity entity : entities)
	{
		level.EnsureExistenceOfComponent<Selected>(entity);
	}
}

class SelectEntities : public MergeableUndoableAction<SelectEntities>
{
private:
	Level& level_;
	std::vector<Entity> entities_to_select_;
	std::vector<Entity> entities_to_deselect_;

	friend class SelectEntities;

public:
	SelectEntities(Level& level, std::vector<Entity>&& entities_to_select, std::vector<Entity>&& entities_to_deselect) :
		level_(level),
		entities_to_select_(entities_to_select),
		entities_to_deselect_(entities_to_deselect)
	{
	}
	void Do()
	{
		Select(level_, entities_to_select_);
		Deselect(level_, entities_to_deselect_);
	}
	void Undo()
	{
		Select(level_, entities_to_deselect_);
		Deselect(level_, entities_to_select_);
	}
	bool TryMerge(const SelectEntities& next_action) override
	{
		entities_to_select_.insert(entities_to_select_.end(), next_action.entities_to_select_.begin(), next_action.entities_to_select_.end());
		entities_to_deselect_.insert(entities_to_deselect_.end(), next_action.entities_to_deselect_.begin(), next_action.entities_to_deselect_.end());
		return true;
	}
};