#include "components/draw_info.hpp"
#include "components/intersection.hpp"
#include "components/laser.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/scheduled_delete.hpp"
#include "components/sound_info.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"

void KillOnIntersectionSystem::Update(Level& level, float dt)
{
	if (level.GetMode() == EDIT_MODE) { return; }

	for (auto& [entity, intersection] : level.GetEntitiesWith<Intersection>())
	{
		if (level.ComputeState() != PLAYING && level.HasComponents<Player>(entity)) { continue; } //Needed to make other particles die after entering goal
		for (auto& i : intersection->intersecting_entities)
		{
			if (!level.HasComponents<KillOnIntersection>(i)) { continue; }

			if (SoundInfo* sound_info = level.GetComponent<SoundInfo>(i))
			{
				sound_info->play_sound.push(DEFAULT);
			}
			if (Face* face = level.RawGetComponent<Face>(entity))
			{
				face->image_path = "content\\textures\\face_dead.png";
			}
			level.RemoveComponents<Intersection>(entity);
			level.RemoveComponents<Velocity>(entity);
			level.RemoveComponents<Player>(entity);

			level.EnsureExistenceOfComponent<ScheduledDelete>(entity)->delete_at = globals.time + 2;
			Shader* shader = level.EnsureExistenceOfComponent<Shader>(entity);
			shader->vertex_shader_path = "shaders\\zapped.vert";
			shader->float_uniforms["start_animation"] = globals.time;
		}
	}
}
