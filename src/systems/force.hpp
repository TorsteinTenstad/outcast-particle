
#include "components/physics.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "systems/electric_force.hpp"
#include "utils.hpp"

static sf::Vector2f CalcAcceleration(ReceivedForces entity, float acceleration_limit)
{
	sf::Vector2f total_force = entity.electric_force + entity.player_force + entity.electric_field_force + entity.magnetic_field_force;
	sf::Vector2f acceleration = total_force / entity.mass;

	float magnitude = Magnitude(acceleration);
	if (magnitude > acceleration_limit)
	{
		acceleration *= acceleration_limit / magnitude;
	}
	return acceleration;
}

class ForceSystem : public GameSystem
{
private:
	float global_max_acceleration_ = 10000;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& acceleration_map = level.GetComponent<Acceleration>();
		auto& received_forces_map = level.GetComponent<ReceivedForces>();

		for (auto const& [entity_id, received_forces] : received_forces_map)
		{
			acceleration_map[entity_id].acceleration = CalcAcceleration(received_forces, global_max_acceleration_);
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};
