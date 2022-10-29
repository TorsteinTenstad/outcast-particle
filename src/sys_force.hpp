#include "PCH.hpp"
#include "comp_physics.hpp"
#include "sys_electric_force.hpp"
#include "utilityfunctions.hpp"

static sf::Vector2f CalcAcceleration(ReceivedForces entity, float acceleration_limit)
{
	sf::Vector2f total_force = entity.electric_force + entity.player_force;
	sf::Vector2f acceleration = total_force / entity.mass;

	float magnitude = Magnitude(acceleration);
	if (magnitude > acceleration_limit)
	{
		acceleration *= acceleration_limit / magnitude;
	}
	return acceleration;
}

class ForceSystem
{
private:
	float global_max_acceleration_ = 10000;

public:
	void Update(std::map<int, Acceleration>& acceleration_map, std::map<int, ReceivedForces>& forces_map)
	{
		for (auto const& [entity_id, received_forces] : forces_map)
		{
			acceleration_map[entity_id].acceleration = CalcAcceleration(received_forces, global_max_acceleration_);
		}
	}
};
