#include "PCH.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "systems/electric_force.hpp"
#include "utilityfunctions.hpp"

static sf::Vector2f CalcAcceleration(ReceivedForces entity, float acceleration_limit)
{
	sf::Vector2f total_force = entity.electric_force + entity.player_force;
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
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, Acceleration>& acceleration_map = level.acceleration_;
		std::map<int, ReceivedForces>& received_forces_map = level.received_forces_;

		for (auto const& [entity_id, received_forces] : received_forces_map)
		{
			acceleration_map[entity_id].acceleration = CalcAcceleration(received_forces, global_max_acceleration_);
		}
	}
};
