#include "components/physics.hpp"
#include "components/player.hpp"
#include "game_system.hpp"
#include "level.hpp"

class PlayerSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)dt;
		std::map<int, ReceivedForces>& received_forces_map = level.GetComponent<ReceivedForces>();
		std::map<int, Player>& player_map = level.GetComponent<Player>();

		for (auto& [entity_id, player] : player_map)
		{
			int x_direction = 0;
			if (cursor_and_keys.key_down[sf::Keyboard::A])
			{
				x_direction -= 1;
			}
			if (cursor_and_keys.key_down[sf::Keyboard::D])
			{
				x_direction += 1;
			}
			int y_direction = 0;
			if (cursor_and_keys.key_down[sf::Keyboard::W])
			{
				y_direction -= 1;
			}
			if (cursor_and_keys.key_down[sf::Keyboard::S])
			{
				y_direction += 1;
			}
			if (cursor_and_keys.key_pressed_this_frame[sf::Keyboard::Space])
			{
				float& charge = level.GetComponent<Charge>()[entity_id].charge;
				charge = -charge;
				if (charge > 0)
				{
					level.GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_blue+.png" };
				}
				else if (charge < 0)
				{
					level.GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_blue-.png" };
				}
				else
				{
					level.GetComponent<DrawInfo>()[entity_id] = { "content\\particle_100_blue.png" };
				}
			}
			received_forces_map[entity_id].player_force.x = x_direction * player.move_force;
			received_forces_map[entity_id].player_force.y = y_direction * player.move_force;
		}
	}
};