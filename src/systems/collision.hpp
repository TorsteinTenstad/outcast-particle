#include "PCH.hpp"
#include "components/collision.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

class CollisionSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& intersection_map = level.GetComponent<Intersection>();
		auto& collision_map = level.GetComponent<Collision>();
		auto& position_map = level.GetComponent<Position>();
		auto& velocity_map = level.GetComponent<Velocity>();
		auto& width_and_height_map = level.GetComponent<WidthAndHeight>();
		auto& radius_map = level.GetComponent<Radius>();
		auto& sound_info_map = level.GetComponent<SoundInfo>();
		for (auto& [entity_id, collision] : collision_map)
		{
			if (intersection_map.count(entity_id) != 0)
			{
				assert(radius_map.count(entity_id) > 0);
				for (auto& intersecting_id : intersection_map[entity_id].intersecting_ids)
				{
					if (collision_map.count(intersecting_id) != 0 && width_and_height_map.count(intersecting_id) != 0)
					{
						float compound_bounce_factor = collision.bounce_factor * collision_map[intersecting_id].bounce_factor;
						float compound_friction = collision.friction + collision_map[intersecting_id].friction;
						float collision_sound_factor = 0;
						if (compound_bounce_factor < 0)
						{
							compound_friction = 0;
						}
						sf::Vector2f distance = position_map[entity_id].position - position_map[intersecting_id].position;
						sf::Vector2f& v = velocity_map[entity_id].velocity;
						float w_radius = width_and_height_map[intersecting_id].width_and_height.x / 2;
						float h_radius = width_and_height_map[intersecting_id].width_and_height.y / 2;
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
							position_map[entity_id].position.y = position_map[intersecting_id].position.y + Sign(distance.y) * (radius_map[entity_id].radius + h_radius);
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
							position_map[entity_id].position.x = position_map[intersecting_id].position.x + Sign(distance.x) * (radius_map[entity_id].radius + w_radius);
						}
						else
						{
							sf::Vector2f distance_from_corner = distance - sf::Vector2f(Sign(distance.x) * w_radius, Sign(distance.y) * h_radius);
							if (Magnitude(distance_from_corner) < radius_map[entity_id].radius)
							{
								sf::Vector2f corner_direction_vector = Normalized(distance_from_corner);
								sf::Vector2f v_corner_direction_component = corner_direction_vector * Dot(v, corner_direction_vector);
								collision_sound_factor = Magnitude(v_corner_direction_component) / 1000;
								position_map[entity_id].position -= distance_from_corner;
								position_map[entity_id].position += (radius_map[entity_id].radius) * corner_direction_vector;
								v = v - (1 + compound_bounce_factor) * v_corner_direction_component;
							}
						}

						assert(sound_info_map.count(intersecting_id) > 0);
						if (collision_sound_factor > 0.1)
						{
							if (collision_sound_factor > 1)
							{
								collision_sound_factor = 1;
							}
							sound_info_map[intersecting_id].sound_volume = collision_sound_factor * collision_sound_factor;
							sound_info_map[intersecting_id].play_sound = true;
						}
					}
				}
			}
		}
	}
	void OnEnterMode(Mode mode) {};
	void OnExitMode(Mode mode) {};
	void OnEnterLevel(int level_id) {};
	void OnExitLevel(int level_id) {};
};