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

	for (auto& [entity_id, intersection] : level.GetEntitiesWith<Intersection>())
	{
		if (level.ComputeState() != PLAYING && level.HasComponents<Player>(entity_id)) { continue; } //Needed to make other particles die after entering goal
		for (auto& i : intersection->intersecting_ids)
		{
			if (!level.HasComponents<KillOnIntersection>(i)) { continue; }

			if (SoundInfo* sound_info = level.GetComponent<SoundInfo>(i))
			{
				sound_info->play_sound.push(DEFAULT);
			}
			if (Face* face = level.RawGetComponent<Face>(entity_id))
			{
				face->image_path = "content\\textures\\face_dead.png";
			}
			level.RemoveComponents<Intersection>(entity_id);
			level.RemoveComponents<Velocity>(entity_id);
			level.RemoveComponents<Player>(entity_id);

			level.AddComponent<ScheduledDelete>(entity_id)->delete_at = globals.time + 2;
			Shader* shader = level.EnsureExistenceOfComponent<Shader>(entity_id);
			shader->vertex_shader_path = "shaders\\zapped.vert";
			shader->float_uniforms["start_animation"] = globals.time;
		}
	}
}
