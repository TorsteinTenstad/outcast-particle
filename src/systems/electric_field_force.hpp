#pragma once
#include "PCH.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2f CalculateElectricFieldForce(Charge particle, ElectricField electric_field)
{
	return (electric_field.field_vector * particle.charge);
}

class ElectricFieldForceSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& intersection_map = level.GetComponent<Intersection>();
		auto& electric_field_vector_map = level.GetComponent<ElectricField>();
		auto& received_forces_map = level.GetComponent<ReceivedForces>();
		auto& charge_map = level.GetComponent<Charge>();
		for (auto& [entity_id, received_forces] : received_forces_map)
		{
			if (intersection_map.count(entity_id) != 0 && charge_map.count(entity_id) != 0)
			{
				sf::Vector2f electric_field_force;
				for (auto& intersection_id : intersection_map[entity_id].intersecting_ids)
				{
					if (electric_field_vector_map.count(intersection_id) != 0)
					{
						electric_field_force += CalculateElectricFieldForce(charge_map[entity_id], electric_field_vector_map[intersection_id]);
					}
				}
				received_forces.electric_field_force = electric_field_force;
			}
		}
	}
	void OnEnterMode(Level& level, Mode mode) {};
	void OnExitMode(Level& level, Mode mode) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};