#include "components/trail.hpp"
#include "components/creation_data.hpp"
#include "components/physics.hpp"
#include "components/size.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"

class TemporaryTrail
{};

void TrailSystem::Update(Level& level, float dt)
{
	for (auto& [entity, trail, radius, velocity, creation_data] : level.GetEntitiesWith<Trail, Radius, Velocity, CreationData>())
	{
		if (globals.time - creation_data->creation_time < 0.3) { return; } // Because of pop-in animation

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
	auto mode = level.GetMode();
	if (mode == READY_MODE || (is_in_level_editing_ && mode != PLAY_MODE))
	{
		for (auto [entity, radius, velocity, creation_data] : level.GetEntitiesWith<Radius, Velocity, CreationData>())
		{
			if (level.HasComponents<Trail>(entity)) { continue; }
			level.AddComponents<Trail, TemporaryTrail>(entity);
		}
	}
	else
	{
		for (auto& [entity, edit_mode_only_trail, trail, radius, velocity, creation_data] : level.GetEntitiesWith<TemporaryTrail, Trail, Radius, Velocity, CreationData>())
		{
			level.RemoveComponents<Trail, TemporaryTrail>(entity);
		}
	}
}