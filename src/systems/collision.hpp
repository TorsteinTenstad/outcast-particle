#include "PCH.hpp"
#include "components/collision.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utilityfunctions.hpp"

static void Collide(Position position_dynamic, Velocity velocity_dynamic, Position position_static, WidthAndHeight width_and_height_static)
{
}

class CollisionSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, Intersection>& intersection_map = level.GetComponent<Intersection>();
		std::map<int, Collision>& collision_map = level.GetComponent<Collision>();
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, Velocity>& velocity_map = level.GetComponent<Velocity>();
		std::map<int, WidthAndHeight>& width_and_height_map = level.GetComponent<WidthAndHeight>();
		for (auto& [entity_id, collision] : collision_map)
		{
			if (intersection_map.count(entity_id) != 0)
			{
				for (auto& intersection_id : intersection_map[entity_id].intersecting_ids)
				{
					if (collision_map.count(intersection_id) != 0 && width_and_height_map.count(intersection_id) != 0)
					{
						//Collide(position_map[entity_id], velocity_map[entity_id], position_map[intersection_id],width_and_height_map[intersection_id]);
						sf::Vector2f distance = position_map[entity_id].position - position_map[intersection_id].position;
						if (abs(distance.x) - width_and_height_map[intersection_id].width_and_height.x / 2 < abs(distance.y) - width_and_height_map[intersection_id].width_and_height.y / 2)
						{
							if (distance.y * velocity_map[entity_id].velocity.y < 0)
							{
								velocity_map[entity_id].velocity.y *= -1;
							}
						}
						else
						{
							if (distance.x * velocity_map[entity_id].velocity.x < 0)
							{
								velocity_map[entity_id].velocity.x *= -1;
							}
						}
					}
				}
			}
		}
	}
};
