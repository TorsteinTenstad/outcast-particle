#include "components/physics.hpp"
#include "systems/_pure_DO_systems.hpp"

void AccelerationSystem::Update(Level& level, float dt)
{
	for (auto const& [entity, acceleration, velocity] : level.GetEntitiesWith<Acceleration, Velocity>())
	{
		velocity->velocity += acceleration->acceleration * dt;
	}
}