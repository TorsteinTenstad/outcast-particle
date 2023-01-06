#pragma once
#include "components/physics.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2f CalculateElectricForce(Position particle_to, Position particle_from, Charge charge_to, Charge charge_from)
{
	float distance = Magnitude(particle_to.position - particle_from.position);
	return ((charge_to.charge * charge_from.charge * (particle_to.position - particle_from.position)) / (distance * distance * distance));
}

class ElectricForceSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& position_map = level.GetComponent<Position>();
		auto& received_forces_map = level.GetComponent<ReceivedForces>();
		auto& charge_map = level.GetComponent<Charge>();

		for (auto& [entity_id_to, entity_forces] : received_forces_map)
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
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};