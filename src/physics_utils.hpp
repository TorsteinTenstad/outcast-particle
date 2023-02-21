#include "SFML/System/Vector2.hpp"
#include "components/physics.hpp"

sf::Vector2f CalculateElectricForce(Position* particle_to, Position* particle_from, Charge* charge_to, Charge* charge_from);