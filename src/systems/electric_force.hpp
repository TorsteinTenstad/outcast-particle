#pragma once
#include "components/physics.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2f CalculateElectricForce(Position* particle_to, Position* particle_from, Charge* charge_to, Charge* charge_from)
{
	float distance = Magnitude(particle_to->position - particle_from->position);
	return ((charge_to->charge * charge_from->charge * (particle_to->position - particle_from->position)) / (distance * distance * distance));
}

class ElectricForceSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
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
};