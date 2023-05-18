#pragma once
#include "components/editable.hpp"
#include "components/goal.hpp"
#include "components/player.hpp"
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class DeleteSelected : public UndoableAction
{
private:
	Level& level_;
	std::vector<Entity> entities_;

public:
	DeleteSelected(Level& level) :
		level_(level)
	{
		for (Entity entity : level.GetEntitiesWithComponent<Selected>())
		{
			// There should be at least 1 Goal and Player in the level, so we don't allow deletion of all
			bool player_found = false;
			bool goal_found = false;
			if (level.HasComponents<Player>(entity) && !player_found)
			{
				player_found = true;
				continue;
			}
			else if (level.HasComponents<Goal>(entity))
			{
				goal_found = true;
				continue;
			}
			else
			{
				entities_.push_back(entity);
			}
		}
	}
	void Do()
	{
		level_.DeactivateEntities(entities_);
	}
	void Undo()
	{
		level_.ActivateEntities(entities_);
	}
};