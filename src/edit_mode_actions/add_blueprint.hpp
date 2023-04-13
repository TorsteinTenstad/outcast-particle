#pragma once
#include "SFML/System/Vector2.hpp"
#include "level.hpp"
#include "undo_system.hpp"
#include <vector>

class AddBlueprint : public UndoableAction
{
private:
	Level& level_;
	int entity;

public:
	AddBlueprint(Level& level, Blueprint blueprint, sf::Vector2f position) :
		level_(level)
	{
		entity = level.AddBlueprint(blueprint);
		level.GetComponent<Position>(entity)->position = position;
		level.DeactivateEntity(entity);
	}
	void Do()
	{
		level_.ActivateEntity(entity);
		level_.EnsureExistenceOfComponent<Selected>(entity);
	}
	void Undo()
	{
		level_.DeactivateEntity(entity);
	}
};