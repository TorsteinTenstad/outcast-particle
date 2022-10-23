#pragma once
#include "charge_component.hpp"
#include "distance.hpp"
#include "physics_components.hpp"
#include "received_forces_component.hpp"

static sf::Vector2f CalculateElectricForce(Position particle_to, Position particle_from, Charge charge_a, Charge charge_b)
{
	float distance = Distance(particle_to.position, particle_from.position);
	return ((charge_a.charge * charge_b.charge * (particle_to.position - particle_from.position)) / (distance * distance * distance));
}

class ElectricForceSystem
{
public:
	void Update(std::map<int, Position>& position_map, std::map<int, Charge>& charge_map, std::map<int, ReceivedForces>& forces_map)
	{
		for (auto& [entity_id_to, entity_forces] : forces_map)
		{
			sf::Vector2f electric_force;
			for (auto const& [entity_id_from, entity_charge] : charge_map)
			{
				if (entity_id_to != entity_id_from)
				{
					electric_force = electric_force + CalculateElectricForce(position_map[entity_id_to], position_map[entity_id_from], charge_map[entity_id_to], entity_charge);
				}
			}
			entity_forces.electric_force = electric_force;
		}
	}
};