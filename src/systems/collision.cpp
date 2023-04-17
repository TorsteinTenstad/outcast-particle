#include "components/collision.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/sound_info.hpp"
#include "globals.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include <cassert>

void CollisionSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != PLAY_MODE)
	{
		return;
	}

	for (auto& [entity_id, intersection, collision, velocity, position, radius] : level.GetEntitiesWith<Intersection, Collision, Velocity, Position, Radius>())
	{
		for (auto& intersecting_id : intersection->intersecting_ids)
		{
			Collision* intersecting_entity_collision = level.RawGetComponent<Collision>(intersecting_id);
			WidthAndHeight* intersecting_entity_width_and_height = level.RawGetComponent<WidthAndHeight>(intersecting_id);
			Position* intersecting_entity_position = level.RawGetComponent<Position>(intersecting_id);
			if (!intersecting_entity_collision
				|| !intersecting_entity_width_and_height
				|| !intersecting_entity_position)
			{
				continue;
			}
			float compound_bounce_factor = collision->bounce_factor * intersecting_entity_collision->bounce_factor;
			float compound_friction = collision->friction + intersecting_entity_collision->friction;
			float collision_sound_factor = 0;
			if (compound_bounce_factor < 0)
			{
				compound_friction = 0;
			}
			sf::Vector2f distance = position->position - intersecting_entity_position->position;
			sf::Vector2f& v = velocity->velocity;
			float w_radius = intersecting_entity_width_and_height->width_and_height.x / 2;
			float h_radius = intersecting_entity_width_and_height->width_and_height.y / 2;
			float radius_used_for_repositioning = 1.001 * radius->radius;
			if (abs(distance.x) < w_radius)
			{
				collision_sound_factor = abs(v.y) / 1000;
				v.y *= -compound_bounce_factor;
				float velocity_lost_to_friction = Sign(v.x) * compound_friction * dt;
				if (abs(v.x) < abs(velocity_lost_to_friction))
				{
					v.x = 0;
				}
				else
				{
					v.x -= velocity_lost_to_friction;
				}
				position->position.y = intersecting_entity_position->position.y + Sign(distance.y) * (radius_used_for_repositioning + h_radius);
			}
			else if (abs(distance.y) < h_radius)
			{
				collision_sound_factor = abs(v.x) / 1000;
				v.x *= -compound_bounce_factor;
				float velocity_lost_to_friction = Sign(v.y) * compound_friction * dt;
				if (abs(v.y) < abs(velocity_lost_to_friction))
				{
					v.y = 0;
				}
				else
				{
					v.y -= velocity_lost_to_friction;
				}
				position->position.x = intersecting_entity_position->position.x + Sign(distance.x) * (radius_used_for_repositioning + w_radius);
			}
			else
			{
				sf::Vector2f distance_from_corner = distance - sf::Vector2f(Sign(distance.x) * w_radius, Sign(distance.y) * h_radius);
				if (Magnitude(distance_from_corner) < radius->radius)
				{
					sf::Vector2f corner_direction_vector = Normalized(distance_from_corner);
					sf::Vector2f v_corner_direction_component = corner_direction_vector * Dot(v, corner_direction_vector);
					collision_sound_factor = Magnitude(v_corner_direction_component) / 1000;
					position->position -= distance_from_corner;
					position->position += (radius_used_for_repositioning)*corner_direction_vector;
					v = v - (1 + compound_bounce_factor) * v_corner_direction_component;
				}
			}

			if (SoundInfo* intersecting_entity_sound_info = level.RawGetComponent<SoundInfo>(intersecting_id))
			{
				if (collision_sound_factor > 0.1)
				{
					if (collision_sound_factor > 1)
					{
						collision_sound_factor = 1;
					}
					intersecting_entity_sound_info->sound_volume = collision_sound_factor * collision_sound_factor;
					intersecting_entity_sound_info->play_sound = true;
				}
			}
		}
	}
}