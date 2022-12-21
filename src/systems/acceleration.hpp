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
	void OnEnterMode(Level& level, Mode mode) {};
	void OnExitMode(Level& level, Mode mode) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};