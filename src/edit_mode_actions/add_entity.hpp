#pragma once
#include "SFML/System/Vector2.hpp"
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class AddEntity : public UndoableAction
{
private:
	Level& level_;
	int entity_;

public:
	AddEntity(Level& level, int entity) :
		level_(level),
		entity_(entity)
	{
		level.DeactivateEntity(entity);
	}
	void Do()
	{
		level_.ActivateEntity(entity_);
	}
	void Undo()
	{
		level_.DeactivateEntity(entity_);
	}
};