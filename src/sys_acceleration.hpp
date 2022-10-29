#include "comp_physics.hpp"

class AccelerationSystem
{
public:
	void Update(std::map<int, Velocity>& velocity_map, std::map<int, Acceleration>& acceleration_map, float dt)
	{
		for (auto const& [entity_id, entity_acceleration] : acceleration_map)
		{
			velocity_map[entity_id].velocity = velocity_map[entity_id].velocity + entity_acceleration.acceleration * dt;
		}
	}
};