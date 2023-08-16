#pragma once
#include "components/editable.hpp"
#include "components/physics.hpp"
#include "level.hpp"
#include "undo_system.hpp"
#include "utils/math.hpp"
#include <vector>

class RotateSelectedFields : public MergeableUndoableAction<RotateSelectedFields>
{
private:
	Level& level_;
	std::vector<Entity> entities_;
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
		for (Entity entity : entities_)
		{
			sf::Vector2f& field_vector = level_.GetComponent<ElectricField>(entity)->field_vector;
			field_vector = GetRotated(field_vector, radians);
		}
	}

public:
	void Do() { Rotate(radians_); }
	void Undo() { Rotate(-radians_); }

	bool TryMerge(const RotateSelectedFields& next_action) override
	{
		if (next_action.entities_ != entities_)
		{
			return true;
		}
		radians_ += next_action.radians_;
		return false;
	}
};