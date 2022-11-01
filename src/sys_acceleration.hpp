#include "game_system.hpp"
#include "level.hpp"

class AccelerationSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		std::map<int, Velocity>& velocity_map = level.velocity_;
		std::map<int, Acceleration>& acceleration_map = level.acceleration_;
		{
			for (auto const& [entity_id, entity_acceleration] : acceleration_map)
			{
				velocity_map[entity_id].velocity = velocity_map[entity_id].velocity + entity_acceleration.acceleration * dt;
			}
		}
	}
};