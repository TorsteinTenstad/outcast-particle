#include "comp_physics.hpp"
#include "comp_player.hpp"

class PlayerSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, std::map<int, Player>& player_map, std::map<int, ReceivedForces>& received_forces_map)
	{
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
			received_forces_map[entity_id].player_force.x = x_direction * player.move_force;
			received_forces_map[entity_id].player_force.y = y_direction * player.move_force;
		}
	}
};