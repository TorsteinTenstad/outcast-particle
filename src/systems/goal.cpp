
#include "components/goal.hpp"
#include "components/collision.hpp"
#include "components/draw_info.hpp"
#include "components/intersection.hpp"
#include "components/player.hpp"
#include "components/scheduled_delete.hpp"
#include "components/screen_wide_shader_effects.hpp"
#include "components/size.hpp"
#include "components/sound_info.hpp"
#include "components/trail.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include "utils/physics.hpp"
#include <cmath>
#include <iostream>

void GoalSystem::Update(Level& level, float dt)
{
	if (level.GetMode() == EDIT_MODE) { return; }
	for (auto& [entity, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
	{
		for (auto intersecting_id : intersection->entities_entered_this_frame)
		{
			if (!level.HasComponents<Goal>(intersecting_id))
			{
				continue;
			}
			if (SoundInfo* sound_info = level.RawGetComponent<SoundInfo>(intersecting_id))
			{
				sound_info->play_sound.push(DEFAULT);
			}
			else
			{
				assert(false);
			}
			level.RemoveComponents<Player>(entity);
			level.RemoveComponents<Collision>(entity);
			level.RemoveComponents<ForceVisualization>(entity);
			level.AddComponent<ScheduledDelete>(entity)->delete_at = globals.time + 2;
			level.GetComponent<Goal>(intersecting_id)->is_goal = true;
			break;
		}
	}
	for (auto& [entity, intersection, radius, velocity, position] : level.GetEntitiesWith<Intersection, Radius, Velocity, Position>())
	{
		for (auto intersecting_id : intersection->intersecting_entities)
		{
			if (!level.HasComponents<Wormhole>(intersecting_id)) { continue; }
			level.RemoveComponents<Acceleration, ReceivedForces, Trail, Charge>(entity);
			FillColor* fill_color = level.EnsureExistenceOfComponent<FillColor>(entity);

			assert(level.HasComponents<Position>(intersecting_id));
			assert(level.HasComponents<Radius>(intersecting_id));
			Position* wormhole_position = level.GetComponent<Position>(intersecting_id);
			Radius* wormhole_radius = level.GetComponent<Radius>(intersecting_id);
			sf::Vector2f pos_offset = wormhole_position->position - position->position;
			float relative_depth = std::min(1.f, Magnitude(pos_offset) / wormhole_radius->radius);
			float pos_angle = Angle(pos_offset);
			float vel_angle = Angle(velocity->velocity);
			float vel_magnitude = Magnitude(velocity->velocity);
			float relative_angle = vel_angle - pos_angle;
			float spiral_theta = 40 * PI * dt;
			if (abs(relative_angle) > PI)
			{
				vel_angle = pos_angle - Sign(relative_angle) * (PI / 2 - spiral_theta);
			}
			else if (abs(relative_angle) > (PI / 2 - spiral_theta))
			{
				vel_angle = pos_angle + Sign(relative_angle) * (PI / 2 - spiral_theta);
			}
			else
			{
				vel_angle = vel_angle - relative_angle * 2 * dt;
			}

			radius->radius = 108 * std::sqrt(relative_depth);

			float target_vel_magnitude = std::sqrt(relative_depth) * 8 * radius->radius;
			float snap = 8 * dt;
			vel_magnitude = (1 - snap) * vel_magnitude + (snap)*target_vel_magnitude;
			velocity->velocity = Vector2fFromPolar(vel_magnitude, vel_angle);
			float color_val = sf::Uint8(255.f * Smoothstep(0.02, 0.1, relative_depth));
			fill_color->color = sf::Color(color_val, color_val, color_val, color_val);
		}
	}
}
