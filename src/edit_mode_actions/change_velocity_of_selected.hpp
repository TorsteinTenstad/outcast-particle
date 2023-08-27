#pragma once
#include "components/physics.hpp"
#include "editable.hpp"
#include "level.hpp"
#include "undo_system.hpp"
#include "utils/container_operations.hpp"
#include "utils/math.hpp"
#include <vector>

class ChangeVelocityOfSelected : public MergeableUndoableAction<ChangeVelocityOfSelected>
{
private:
	Level& level_;
	std::vector<Entity> entities_;
	std::vector<sf::Vector2f> original_velocities_;
	float magnitude_delta = 0;
	float angle_delta = 0;

	friend class ChangeVelocityOfSelected;

public:
	ChangeVelocityOfSelected(Level& level, float magnitude_delta, float angle_delta) :
		level_(level),
		magnitude_delta(magnitude_delta),
		angle_delta(angle_delta)
	{
		for (auto [entity, selected, velocity] : level.GetEntitiesWith<Selected, Velocity>())
		{
			assert(level.HasComponents<Editable>(entity));
			entities_.push_back(entity);
			original_velocities_.push_back(velocity->velocity);
		}
	}
	void Do()
	{
		for (const auto& entity : entities_)
		{
			sf::Vector2f& v = level_.GetComponent<Velocity>(entity)->velocity;
			float magnitude = Magnitude(v);
			float angle = Angle(v);
			magnitude += magnitude_delta;
			angle += angle_delta;
			v = Vector2fFromPolar(magnitude, angle);
		}
	}
	void Undo()
	{
		for (const auto& [entity, original_value] : zip(entities_, original_velocities_))
		{
			level_.GetComponent<Velocity>(entity)->velocity = original_value;
		}
	}

	bool
	TryMerge(const ChangeVelocityOfSelected& next_action) override
	{
		if (next_action.entities_ != entities_) { return false; }
		magnitude_delta += next_action.magnitude_delta;
		angle_delta += next_action.angle_delta;
		return true;
	}
};