#pragma once
#include "PCH.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

static sf::Vector2f CalculateElectricFieldForce(Charge particle, ElectricField electric_field)
{
	return (electric_field.field_vector * particle.charge);
}

class ElectricFieldForceSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, Intersection>& intersection_map = level.GetComponent<Intersection>();
		std::map<int, ElectricField>& electric_field_vector_map = level.GetComponent<ElectricField>();
		std::map<int, ReceivedForces>& received_forces_map = level.GetComponent<ReceivedForces>();
		std::map<int, Charge>& charge_map = level.GetComponent<Charge>();
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
};