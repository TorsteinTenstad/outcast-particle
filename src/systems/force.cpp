
#include "systems/_pure_DO_systems.hpp"
#include "components/physics.hpp"
#include "utils/math.hpp"

static sf::Vector2f CalcAcceleration(ReceivedForces* received_forces, Mass* mass, float acceleration_limit)
{
	sf::Vector2f total_force =
		received_forces->electric_force
		+ received_forces->player_force
		+ received_forces->electric_field_force
		+ received_forces->magnetic_field_force
		+ received_forces->gravitational_force;
	sf::Vector2f acceleration = total_force / mass->mass;

	float magnitude = Magnitude(acceleration);
	if (magnitude > acceleration_limit)
	{
		acceleration *= acceleration_limit / magnitude;
	}
	return acceleration;
}

void ForceSystem::Update(Level& level, float dt)
{
	for (auto const& [entity_id, received_forces, mass, acceleration] : level.GetEntitiesWith<ReceivedForces, Mass, Acceleration>())
	{
		acceleration->acceleration = CalcAcceleration(received_forces, mass, GLOBAL_MAX_ACCELERATION);
	}
}
