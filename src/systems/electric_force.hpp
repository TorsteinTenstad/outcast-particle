#pragma once
#include "components/physics.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

static sf::Vector2f CalculateElectricForce(Position particle_to, Position particle_from, Charge charge_a, Charge charge_b)
{
	float distance = Magnitude(particle_to.position - particle_from.position);
	return ((charge_a.charge * charge_b.charge * (particle_to.position - particle_from.position)) / (distance * distance * distance));
}

class ElectricForceSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, ReceivedForces>& received_forces_map = level.GetComponent<ReceivedForces>();
		std::map<int, Charge>& charge_map = level.GetComponent<Charge>();
		std::map<int, VisualizedForces>& visualized_forces_map = level.GetComponent<VisualizedForces>();

		for (auto& [entity_id_to, entity_forces] : received_forces_map)
		{
			if (visualized_forces_map.count(entity_id_to))
			{
				visualized_forces_map[entity_id_to].forces.clear();
			}
			sf::Vector2f electric_force;
			for (auto const& [entity_id_from, entity_charge] : charge_map)
			{
				if (entity_id_to != entity_id_from)
				{
					sf::Vector2f force_from_entity = CalculateElectricForce(position_map[entity_id_to], position_map[entity_id_from], charge_map[entity_id_to], entity_charge);
					if (visualized_forces_map.count(entity_id_to))
					{
						visualized_forces_map[entity_id_to].forces.push_back(force_from_entity);
					}
					electric_force = electric_force + force_from_entity;
				}
			}
			entity_forces.electric_force = electric_force;
		}
	}
};