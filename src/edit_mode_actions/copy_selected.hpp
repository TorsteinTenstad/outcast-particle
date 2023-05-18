#pragma once
#include "SFML/System/Vector2.hpp"
#include "components/player.hpp"
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class CopySelected : public UndoableAction
{
private:
	Level& level_;
	std::vector<Entity> entities_;

public:
	CopySelected(Level& level, sf::Vector2f origin) :
		level_(level)
	{
		for (auto [entity, selected, position] : level.GetEntitiesWith<Selected, Position>())
		{
			if (level.HasComponents<Player>(entity)) { continue; }
			Entity copy = level.CopyEntity(entity);
			level.GetComponent<Position>(copy)->position = origin + selected->mouse_offset;
			level.RemoveComponents<Selected>(copy);
			level.DeactivateEntities(copy);
			entities_.push_back(copy);
		}
	}
	void Do()
	{
		level_.ActivateEntities(entities_);
	}
	void Undo()
	{
		level_.DeactivateEntities(entities_);
	}
};