#include "game_system.hpp"
#include "level.hpp"

class AccelerationSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto const& [entity_id, acceleration, velocity] : level.GetEntitiesWith<Acceleration, Velocity>())
		{
			velocity->velocity += acceleration->acceleration * dt;
		}
	}
	
};