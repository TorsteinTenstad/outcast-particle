#include "PCH.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"

class VelocitySystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, Velocity>& velocity_map = level.GetComponent<Velocity>();

		for (auto const& [entity_id, entity_velocity] : velocity_map)
		{
			position_map[entity_id].last_frame_position = position_map[entity_id].position;
			position_map[entity_id].position = position_map[entity_id].position + entity_velocity.velocity * dt;
		}
	}
};