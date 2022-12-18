#pragma once
#include "PCH.hpp"
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
		auto& intersection_map = level.GetComponent<Intersection>();
		auto& magnetic_field_strength_map = level.GetComponent<MagneticField>();
		auto& received_forces_map = level.GetComponent<ReceivedForces>();
		auto& charge_map = level.GetComponent<Charge>();
		auto& velocity_map = level.GetComponent<Velocity>();
		for (auto& [entity_id, received_forces] : received_forces_map)
		{
			if (intersection_map.count(entity_id) != 0 && charge_map.count(entity_id) != 0)
			{
				sf::Vector2f magnetic_field_force;
				for (auto& intersection_id : intersection_map[entity_id].intersecting_ids)
				{
					if (magnetic_field_strength_map.count(intersection_id) != 0)
					{
						magnetic_field_force += CalculateMagneticFieldForce(charge_map[entity_id], velocity_map[entity_id], magnetic_field_strength_map[intersection_id]);
					}
				}
				received_forces.magnetic_field_force = magnetic_field_force;
			}
		}
	}
	void OnEnterMode(Mode mode) {};
	void OnExitMode(Mode mode) {};
	void OnEnterLevel(int level_id) {};
	void OnExitLevel(int level_id) {};
};