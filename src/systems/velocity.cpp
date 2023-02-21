#include "_pure_DO_systems.hpp"
#include "components/physics.hpp"

void VelocitySystem::Update(Level& level, float dt)
{
	for (auto const& [entity_id, velocity, position] : level.GetEntitiesWith<Velocity, Position>())
	{
		position->position += velocity->velocity * dt;
	}
}