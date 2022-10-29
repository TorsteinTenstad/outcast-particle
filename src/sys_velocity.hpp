#include "PCH.hpp"
#include "comp_physics.hpp"

class VelocitySystem
{
public:
	void Update(std::map<int, Position>& position_map, std::map<int, Velocity>& velocity_map, float dt)
	{
		for (auto const& [entity_id, entity_velocity] : velocity_map)
		{
			position_map[entity_id].position = position_map[entity_id].position + entity_velocity.velocity * dt;
		}
	}
};