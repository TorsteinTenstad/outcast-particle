#include "utils/physics.hpp"
#include "utils/math.hpp"

static sf::Vector2f CalculateTwoBodyForce(Position* particle_to, Position* particle_from, float strength)
{
	float distance = Magnitude(particle_to->position - particle_from->position);
	return ((strength * (particle_to->position - particle_from->position)) / (distance * distance * distance));
}

sf::Vector2f CalculateElectricForce(Position* particle_to, Position* particle_from, Charge* charge_to, Charge* charge_from)
{
	return CalculateTwoBodyForce(particle_to, particle_from, charge_to->charge * charge_from->charge);
}
sf::Vector2f CalculateGravitationalForce(Position* particle_to, Position* particle_from, float mass_product)
{
	return CalculateTwoBodyForce(particle_to, particle_from, -mass_product);
}