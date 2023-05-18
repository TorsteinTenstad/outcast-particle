#pragma once
#include "SFML/System/Vector2.hpp"
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class AddEntity : public UndoableAction
{
private:
	Level& level_;
	Entity entity_;

public:
	AddEntity(Level& level, Entity entity) :
		level_(level),
		entity_(entity)
	{
		level.DeactivateEntities(entity);
	}
	void Do()
	{
		level_.ActivateEntities(entity_);
	}
	void Undo()
	{
		level_.DeactivateEntities(entity_);
	}
};