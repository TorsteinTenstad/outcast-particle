#include "game_system.hpp"
#include "level.hpp"

class AccelerationSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& velocity_map = level.GetComponent<Velocity>();
		auto& acceleration_map = level.GetComponent<Acceleration>();
		{
			for (auto const& [entity_id, entity_acceleration] : acceleration_map)
			{
				velocity_map[entity_id].velocity = velocity_map[entity_id].velocity + entity_acceleration.acceleration * dt;
			}
		}
	}
	void OnEnterMode(Mode mode) {};
	void OnExitMode(Mode mode) {};
	void OnEnterLevel(int level_id) {};
	void OnExitLevel(int level_id) {};
};