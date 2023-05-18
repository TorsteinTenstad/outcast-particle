#include "components/physics.hpp"
#include "systems/_pure_DO_systems.hpp"

void VelocitySystem::Update(Level& level, float dt)
{
	for (auto const& [entity, velocity, position] : level.GetEntitiesWith<Velocity, Position>())
	{
		position->position += velocity->velocity * dt;
	}
}