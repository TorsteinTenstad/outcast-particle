#pragma once
#include "level.hpp"
#include "undo_system.hpp"
#include "utils/math.hpp"
#include <vector>

class RotateSelectedFields : public UndoableAction
{
private:
	Level& level_;
	std::vector<int> entities_;
	float radians_;

	friend class RotateSelectedFields;

public:
	RotateSelectedFields(Level& level, float radians) :
		level_(level),
		radians_(radians)
	{
		for (auto [entity, selected, width_and_height] : level.GetEntitiesWith<Selected, ElectricField>())
		{
			assert(level.HasComponents<Editable>(entity));
			entities_.push_back(entity);
		}
	}

private:
	void Rotate(float radians)
	{
		for (int entity : entities_)
		{
			sf::Vector2f& field_vector = level_.GetComponent<ElectricField>(entity)->field_vector;
			field_vector = GetRotated(field_vector, radians);
		}
	}

public:
	void Do() { Rotate(radians_); }
	void Undo() { Rotate(-radians_); }

	std::optional<std::unique_ptr<UndoableAction>> TryMerge(std::unique_ptr<UndoableAction> next_action) override
	{
		if (typeid(*this) != typeid(*next_action))
		{
			return next_action;
		}
		auto other_modify_entity_size = static_cast<RotateSelectedFields*>(next_action.get());
		if (other_modify_entity_size->entities_ != entities_)
		{
			return next_action;
		}
		radians_ += other_modify_entity_size->radians_;
		return {};
	}
};