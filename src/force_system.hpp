#include "PCH.hpp"
#include "electric_force_system.hpp"
#include "physics_components.hpp"

static sf::Vector2f TotalForce(ReceivedForces forces)
{
	sf::Vector2f total_force = forces.electric_force + forces.keyboard_force;
	return total_force;
}

class ForceSystem
{
public:
	void Update(std::map<int, Acceleration>& acceleration_map, std::map<int, ReceivedForces>& forces_map)
	{
		for (auto const& [entity_id, received_forces] : forces_map)
		{
			acceleration_map[entity_id].acceleration = TotalForce(received_forces) / received_forces.mass;
		}
	}
};
