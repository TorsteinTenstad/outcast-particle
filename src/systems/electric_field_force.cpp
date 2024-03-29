#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "globals.hpp"
#include "systems/_pure_DO_systems.hpp"

static sf::Vector2f CalculateElectricFieldForce(Charge* particle, ElectricField* electric_field)
{
	return (electric_field->field_vector * particle->charge);
}

void ElectricFieldForceSystem::Update(Level& level, float dt)
{
	for (auto [entity, received_forces, intersection, charge] : level.GetEntitiesWith<ReceivedForces, Intersection, Charge>())
	{
		received_forces->electric_field_force = sf::Vector2f(0, 0);
		for (auto& intersecting_id : intersection->intersecting_entities)
		{
			if (ElectricField* electric_field = level.RawGetComponent<ElectricField>(intersecting_id))
			{
				received_forces->electric_field_force += CalculateElectricFieldForce(charge, electric_field);
			}
		}
	}
}