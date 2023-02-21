#include "physics_utils.hpp"
#include "utils.hpp"

sf::Vector2f CalculateElectricForce(Position* particle_to, Position* particle_from, Charge* charge_to, Charge* charge_from)
{
	float distance = Magnitude(particle_to->position - particle_from->position);
	return ((charge_to->charge * charge_from->charge * (particle_to->position - particle_from->position)) / (distance * distance * distance));
}