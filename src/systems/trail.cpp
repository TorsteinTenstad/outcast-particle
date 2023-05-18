#include "components/trail.hpp"
#include "components/physics.hpp"
#include "components/size.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"

void TrailSystem::Update(Level& level, float dt)
{
	if (globals.time - globals.time_of_last_level_enter < 0.3) // Because of pop-in animation
	{
		return;
	}
	for (auto& [entity, trail, radius, velocity] : level.GetEntitiesWith<Trail, Radius, Velocity>())
	{
		trail->path.insert(trail->path.begin(), -velocity->velocity * dt - (radius->radius / TRAIL_N) * Normalized(velocity->velocity));
		if (trail->path.size() > TRAIL_N)
		{
			trail->path.pop_back();
		}
		if (trail->path.size() != trail->widths.size())
		{
			trail->widths.clear();
			for (unsigned i = 0; i < trail->path.size(); ++i)
			{
				trail->widths.push_back(0.8 * pow((float)(trail->path.size() - i) / trail->path.size(), 1.5));
			}
		}
	}
}