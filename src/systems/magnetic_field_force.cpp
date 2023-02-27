#include "_pure_DO_systems.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2f CalculateMagneticFieldForce(Charge* particle_charge, Velocity* particle_velocity, MagneticField* magnetic_field)
{
	sf::Vector2f magnetic_field_vector = GetQuarterTurnRotation(particle_velocity->velocity);
	return magnetic_field_vector * particle_charge->charge * magnetic_field->field_strength / 1000.f;
}

void MagneticFieldForceSystem::Update(Level& level, float dt)
{
	for (auto& [entity_id, received_forces, intersection, charge, velocity] : level.GetEntitiesWith<ReceivedForces, Intersection, Charge, Velocity>())
	{
		sf::Vector2f magnetic_field_force;
		for (const auto& intersection_id : intersection->intersecting_ids)
		{
			if (level.HasComponents<MagneticField>(intersection_id))
			{
				magnetic_field_force += CalculateMagneticFieldForce(charge, velocity, level.GetComponent<MagneticField>(intersection_id));
			}
		}
		received_forces->magnetic_field_force = magnetic_field_force;
	}
}