#pragma once

#include "components/intersection.hpp"
#include "components/kill_on_intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <iostream>

class KillOnIntersectionSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& intersection_map = level.GetComponent<Intersection>();
		auto& kill_on_intersection_map = level.GetComponent<KillOnIntersection>();
		auto& sound_info_map = level.GetComponent<SoundInfo>();
		auto& face_map = level.GetComponent<Face>();

		for (auto& [entity_id, intersection] : level.GetEntitiesWith<Intersection>())
		{
			for (auto& i : intersection->intersecting_ids)
			{
				if (kill_on_intersection_map.count(i) != 0)
				{
					if (sound_info_map.count(i))
					{
						sound_info_map[i].play_sound = true;
					}
					if (face_map.count(entity_id))
					{
						face_map[entity_id].image_path = "content\\textures\\face_dead.png";
					}
					level.GetComponent<Intersection>().erase(entity_id);
					level.GetComponent<Shader>().erase(entity_id);
					level.GetComponent<Velocity>().erase(entity_id);
					level.GetComponent<Player>().erase(entity_id);

					level.GetComponent<ScheduledDelete>()[entity_id].delete_at = globals.time + 2;
					level.GetComponent<Shader>()[entity_id].vertex_shader_path = "shaders\\zapped.vert";
					level.GetComponent<Shader>()[entity_id].fragment_shader_path = "shaders\\zapped.frag";
					level.GetComponent<Shader>()[entity_id].float_uniforms["start_animation"] = globals.time;
					level.GetComponent<Shader>()[entity_id].float_uniforms["_time"];
				}
			}
		}
	}
	
};