#include "PCH.hpp"
#include "components/collision.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

class CollisionSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		std::map<int, Intersection>& intersection_map = level.GetComponent<Intersection>();
		std::map<int, Collision>& collision_map = level.GetComponent<Collision>();
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, Velocity>& velocity_map = level.GetComponent<Velocity>();
		std::map<int, WidthAndHeight>& width_and_height_map = level.GetComponent<WidthAndHeight>();
		std::map<int, Radius>& radius_map = level.GetComponent<Radius>();
		std::map<int, SoundInfo>& sound_info_map = level.GetComponent<SoundInfo>();
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
						float collision_sound_factor = 1;
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
							sf::Vector2f distance_from_corner = distance;
							distance_from_corner.x -= Sign(distance_from_corner.x) * w_radius;
							distance_from_corner.y -= Sign(distance_from_corner.y) * h_radius;
							sf::Vector2f corner_direction_vector = distance_from_corner / Magnitude(distance_from_corner);
							sf::Vector2f v_corner_direction_component = corner_direction_vector * Dot(v, corner_direction_vector);
							collision_sound_factor = Magnitude(v_corner_direction_component) / 1000;
							position_map[entity_id].position -= distance_from_corner;
							position_map[entity_id].position += (radius_map[entity_id].radius) * distance_from_corner / Magnitude(distance_from_corner);
							v = v - (1 + compound_bounce_factor) * v_corner_direction_component;
						}
						sound_info_map[intersecting_id].play_sound = true;
						if (collision_sound_factor > 1)
						{
							collision_sound_factor = 1;
						}
						sound_info_map[intersecting_id].sound_volume = 100 * collision_sound_factor;
					}
				}
			}
		}
	}
};