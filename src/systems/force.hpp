
#include "components/physics.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "utils.hpp"

static sf::Vector2f CalcAcceleration(ReceivedForces* received_forces, float acceleration_limit)
{
	sf::Vector2f total_force =
		received_forces->electric_force
		+ received_forces->player_force
		+ received_forces->electric_field_force
		+ received_forces->magnetic_field_force;
	sf::Vector2f acceleration = total_force / received_forces->mass;

	float magnitude = Magnitude(acceleration);
	if (magnitude > acceleration_limit)
	{
		acceleration *= acceleration_limit / magnitude;
	}
	return acceleration;
}

class ForceSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (auto const& [entity_id, received_forces, acceleration] : level.GetEntitiesWith<ReceivedForces, Acceleration>())
		{
			acceleration->acceleration = CalcAcceleration(received_forces, GLOBAL_MAX_ACCELERATION);
		}
	}
};
