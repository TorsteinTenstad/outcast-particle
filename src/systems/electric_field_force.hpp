#pragma once

#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2f CalculateElectricFieldForce(Charge* particle, ElectricField* electric_field)
{
	return (electric_field->field_vector * particle->charge);
}

class ElectricFieldForceSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto [entity_id, received_forces, intersection, charge] : level.GetEntitiesWith<ReceivedForces, Intersection, Charge>())
		{
			received_forces->electric_field_force = sf::Vector2f(0, 0);
			for (auto& intersecting_id : intersection->intersecting_ids)
			{
				if (level.HasComponents<ElectricField>(intersecting_id))
				{
					ElectricField* electric_field = level.GetComponent<ElectricField>(intersecting_id);
					received_forces->electric_field_force += CalculateElectricFieldForce(charge, electric_field);
				}
			}
		}
	}
};