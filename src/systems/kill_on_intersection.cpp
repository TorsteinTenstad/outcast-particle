#include "components/kill_on_intersection.hpp"
#include "_pure_DO_systems.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <iostream>

void KillOnIntersectionSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != PLAY_MODE)
	{
		return;
	}

	for (auto& [entity_id, intersection] : level.GetEntitiesWith<Intersection>())
	{
		for (auto& i : intersection->intersecting_ids)
		{
			if (!level.HasComponents<KillOnIntersection>(i))
			{
				continue;
			}
			if (level.HasComponents<SoundInfo>(i))
			{
				level.GetComponent<SoundInfo>(i)->play_sound = true;
			}
			if (level.HasComponents<Face>(entity_id))
			{
				level.GetComponent<Face>(entity_id)->image_path = "content\\textures\\face_dead.png";
			}
			level.RemoveComponents<Intersection>(entity_id);
			level.RemoveComponents<Shader>(entity_id);
			level.RemoveComponents<Velocity>(entity_id);
			level.RemoveComponents<Player>(entity_id);

			level.AddComponent<ScheduledDelete>(entity_id)->delete_at = globals.time + 2;
			Shader* shader = level.AddComponent<Shader>(entity_id);
			shader->vertex_shader_path = "shaders\\zapped.vert";
			shader->fragment_shader_path = "shaders\\zapped.frag";
			shader->float_uniforms["start_animation"] = globals.time;
		}
	}
}
