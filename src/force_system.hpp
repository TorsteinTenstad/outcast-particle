#include "PCH.hpp"
#include "electric_force_system.hpp"
#include "physics_components.hpp"
#include "utilityfunctions.hpp"

static sf::Vector2f CalcAcceleration(ReceivedForces entity)
{
	sf::Vector2f total_force = entity.electric_force + entity.keyboard_force;
	sf::Vector2f acceleration = total_force / entity.mass;

	float magnitude = Magnitude(acceleration);
	if (magnitude > global_max_acceleration_)
	{
		acceleration *= global_max_acceleration_ / magnitude;
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
			acceleration_map[entity_id].acceleration = CalcAcceleration(received_forces);
		}
	}
};
