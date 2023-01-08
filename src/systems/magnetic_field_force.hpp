#pragma once

#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2f CalculateMagneticFieldForce(Charge particle_charge, Velocity particle_velocity, MagneticField magnetic_field)
{
	sf::Vector2f magnetic_field_vector = GetQuarterTurnRotation(particle_velocity.velocity);
	return magnetic_field_vector * particle_charge.charge * magnetic_field.field_strength / 1000.f;
}

class MagneticFieldForceSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& magnetic_field_strength_map = level.GetComponent<MagneticField>();
		for (auto& [entity_id, received_forces, intersection, charge, velocity] : level.GetEntitiesWith<ReceivedForces, Intersection, Charge, Velocity>())
		{
			sf::Vector2f magnetic_field_force;
			for (const auto& intersection_id : intersection->intersecting_ids)
			{
				if (magnetic_field_strength_map.count(intersection_id) != 0)
				{
					magnetic_field_force += CalculateMagneticFieldForce(*charge, *velocity, magnetic_field_strength_map[intersection_id]);
				}
			}
			received_forces->magnetic_field_force = magnetic_field_force;
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};