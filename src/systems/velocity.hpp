
#include "components/physics.hpp"
#include "game_system.hpp"

class VelocitySystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto const& [entity_id, velocity, position] : level.GetEntitiesWith<Velocity, Position>())
		{
			position->position += velocity->velocity * dt;
		}
	}
	
};