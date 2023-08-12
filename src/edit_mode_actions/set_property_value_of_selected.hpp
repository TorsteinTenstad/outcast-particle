#pragma once
#include "Components/sound_info.hpp"
#include "components/collision.hpp"
#include "components/editable.hpp"
#include "components/grid_adaptive_textures.hpp"
#include "components/laser.hpp"
#include "components/physics.hpp"
#include "constants.hpp"
#include "level.hpp"
#include "undo_system.hpp"
#include "utils/container_operations.hpp"
#include "utils/math.hpp"
#include <vector>

static void SetMagnitudeOfFloat(float& x, float magnitude)
{
	x = Sign(x) * magnitude;
}

class SetPropertyValueOfSelected : public MergeableUndoableAction<SetPropertyValueOfSelected>
{
private:
	Level& level_;

	std::optional<int> property_value_idx_;
	int flip_;

	std::vector<Entity> particles_;
	std::vector<float> particles_original_values_;

	std::vector<Entity> magnetic_fields_;
	std::vector<float> magnetic_fields_original_values_;

	std::vector<Entity> electric_fields_;
	std::vector<sf::Vector2f> electric_fields_original_values_;

	std::vector<Entity> lasers_;
	std::vector<float> lasers_period_original_values_;
	std::vector<float> lasers_duty_cycle_original_values_;

	std::vector<Entity> walls_;
	std::vector<float> walls_original_values_;

	friend class SetPropertyValueOfSelected;

public:
	SetPropertyValueOfSelected(Level& level, std::optional<int> property_value_idx, std::optional<int> flip) :
		level_(level),
		property_value_idx_(property_value_idx),
		flip_(flip.value_or(1))
	{
		for (auto [entity, selected, component] : level.GetEntitiesWith<Selected, Charge>())
		{
			particles_.push_back(entity);
			particles_original_values_.push_back(component->charge);
		}
		for (auto [entity, selected, component] : level.GetEntitiesWith<Selected, MagneticField>())
		{
			magnetic_fields_.push_back(entity);
			magnetic_fields_original_values_.push_back(component->field_strength);
		}
		for (auto [entity, selected, component] : level.GetEntitiesWith<Selected, ElectricField>())
		{
			electric_fields_.push_back(entity);
			electric_fields_original_values_.push_back(component->field_vector);
		}
		for (auto [entity, selected, component] : level.GetEntitiesWith<Selected, TogglingLaser>())
		{
			lasers_.push_back(entity);
			lasers_period_original_values_.push_back(component->period_s);
			lasers_duty_cycle_original_values_.push_back(component->duty_cycle);
		}
		for (auto [entity, selected, component, wall] : level.GetEntitiesWith<Selected, Collision, Wall>())
		{
			walls_.push_back(entity);
			walls_original_values_.push_back(component->bounce_factor);
		}
	}
	void Do()
	{
		if (property_value_idx_.has_value())
		{
			int property_value_idx = property_value_idx_.value();
			for (Entity entity : particles_)
			{
				SetMagnitudeOfFloat(level_.GetComponent<Charge>(entity)->charge, PARTICLE_CHARGE_CATEGORIES[property_value_idx]);
			}
			for (Entity entity : magnetic_fields_)
			{
				SetMagnitudeOfFloat(level_.GetComponent<MagneticField>(entity)->field_strength, MAGNETIC_FIELD_STRENGTH_CATEGORIES[property_value_idx]);
			}
			for (Entity entity : electric_fields_)
			{
				sf::Vector2f& v = level_.GetComponent<ElectricField>(entity)->field_vector;
				v = Normalized(v) * ELECTRIC_FIELD_STRENGTH_CATEGORIES[property_value_idx];
			}
			for (Entity entity : lasers_)
			{
				level_.GetComponent<TogglingLaser>(entity)->period_s = TOGGLING_LASER_PERIOD_CATEGORIES[property_value_idx];
				level_.GetComponent<TogglingLaser>(entity)->duty_cycle = TOGGLING_LASER_DUTY_CYCLE_CATEGORIES[property_value_idx];
			}
			for (Entity entity : walls_)
			{
				level_.GetComponent<Collision>(entity)->bounce_factor = WALL_BOUNCE_CATEGORIES[property_value_idx];
			}
		}
		for (Entity entity : particles_)
		{
			level_.GetComponent<Charge>(entity)->charge *= flip_;
		}
		for (Entity entity : magnetic_fields_)
		{
			level_.GetComponent<MagneticField>(entity)->field_strength *= flip_;
		}
	}
	void Undo()
	{
		for (const auto& [entity, original_value] : zip(particles_, particles_original_values_))
		{
			level_.GetComponent<Charge>(entity)->charge = original_value;
		}
		for (const auto& [entity, original_value] : zip(magnetic_fields_, magnetic_fields_original_values_))
		{
			level_.GetComponent<MagneticField>(entity)->field_strength = original_value;
		}
		for (const auto& [entity, original_value] : zip(electric_fields_, electric_fields_original_values_))
		{
			level_.GetComponent<ElectricField>(entity)->field_vector = original_value;
		}
		for (const auto& [entity, period, duty_cycle] : zip(lasers_, lasers_period_original_values_, lasers_duty_cycle_original_values_))
		{
			level_.GetComponent<TogglingLaser>(entity)->period_s = period;
			level_.GetComponent<TogglingLaser>(entity)->duty_cycle = duty_cycle;
		}
		for (const auto& [entity, original_value] : zip(walls_, walls_original_values_))
		{
			level_.GetComponent<Collision>(entity)->bounce_factor = original_value;
		}
	}
	bool TryMerge(const SetPropertyValueOfSelected& next_action) override
	{
		if (next_action.particles_ != particles_
			|| next_action.magnetic_fields_ != magnetic_fields_
			|| next_action.electric_fields_ != electric_fields_
			|| next_action.lasers_ != lasers_
			|| next_action.walls_ != walls_)
		{
			return false;
		}
		property_value_idx_ = next_action.property_value_idx_;
		flip_ *= next_action.flip_;
		return true;
	}
};