#pragma once
#include "_pure_DO_systems.hpp"
#include "components/physics.hpp"
#include "utils/physics.hpp"

void ElectricForceSystem::Update(Level& level, float dt)
{
	for (auto [entity_id_to, received_forces, charge_to, position_to] : level.GetEntitiesWith<ReceivedForces, Charge, Position>())
	{
		received_forces->electric_force = sf::Vector2f(0, 0);
		for (auto [entity_id_from, charge_from, position_from] : level.GetEntitiesWith<Charge, Position>())
		{
			if (entity_id_to != entity_id_from)
			{
				received_forces->electric_force += CalculateElectricForce(position_to, position_from, charge_to, charge_from);
			}
		}
	}
}