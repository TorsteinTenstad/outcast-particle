#include "PCH.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"

class VelocitySystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& position_map = level.GetComponent<Position>();
		auto& velocity_map = level.GetComponent<Velocity>();

		for (auto const& [entity_id, entity_velocity] : velocity_map)
		{
			position_map[entity_id].position = position_map[entity_id].position + entity_velocity.velocity * dt;
		}
	}
};