#include "_pure_DO_systems.hpp"

void AnimatedPositionSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, animated_position, position] : level.GetEntitiesWith<AnimatedPosition, Position>())
	{
		position->position = animated_position->position_at_time_func(globals.time - animated_position->start_time);
	}
}
