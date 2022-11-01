#include "comp_physics.hpp"
#include "comp_player.hpp"
#include "game_system.hpp"
#include "level.hpp"

class PlayerSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)dt;
		std::map<int, ReceivedForces>& received_forces_map = level.received_forces_;
		std::map<int, Player>& player_map = level.player_;

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