#include "game_system.hpp"
#include "level.hpp"

#define TRAIL_N 64

class TrailSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		std::map<int, Velocity>& velocity_map = level.GetComponent<Velocity>();
		std::map<int, Trail>& trail_map = level.GetComponent<Trail>();
		std::map<int, Radius>& radius_map = level.GetComponent<Radius>();

		for (auto& [entity_id, trail] : trail_map)
		{
			assert(radius_map.count(entity_id) > 0);

			trail.path.insert(trail.path.begin(), -velocity_map[entity_id].velocity * dt - (radius_map[entity_id].radius / TRAIL_N) * Normalized(velocity_map[entity_id].velocity));
			if (trail.path.size() > TRAIL_N)
			{
				trail.path.pop_back();
			}
			if (trail.path.size() != trail.widths.size())
			{
				trail.widths.clear();
				for (unsigned i = 0; i < trail.path.size(); ++i)
				{
					trail.widths.push_back(0.9 * radius_map[entity_id].radius * pow((float)(trail.path.size() - i) / trail.path.size(), 1.5));
				}
			}
		}
	}
};