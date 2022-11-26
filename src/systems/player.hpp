#include "components/physics.hpp"
#include "components/player.hpp"
#include "controls_config.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class PlayerSystem : public GameSystem
{
public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)dt;
		std::map<int, ReceivedForces>& received_forces_map = level.GetComponent<ReceivedForces>();
		std::map<int, Player>& player_map = level.GetComponent<Player>();
		std::map<int, Charge>& charge_map = level.GetComponent<Charge>();

		for (auto& [entity_id, player] : player_map)
		{
			int x_direction = 0;
			if (cursor_and_keys.key_down[PLAYER_MOVE_LEFT_KEY])
			{
				x_direction -= 1;
			}
			if (cursor_and_keys.key_down[PLAYER_MOVE_RIGHT_KEY])
			{
				x_direction += 1;
			}
			int y_direction = 0;
			if (cursor_and_keys.key_down[PLAYER_MOVE_UP_KEY])
			{
				y_direction -= 1;
			}
			if (cursor_and_keys.key_down[PLAYER_MOVE_DOWN_KEY])
			{
				y_direction += 1;
			}
			received_forces_map[entity_id].player_force.x = x_direction * player.move_force;
			received_forces_map[entity_id].player_force.y = y_direction * player.move_force;

			if (cursor_and_keys.key_pressed_this_frame[PLAYER_GO_NEUTRAL_KEY] && player.can_go_neutral)
			{
				player_map[entity_id].default_charge = charge_map[entity_id].charge;
				charge_map[entity_id].charge = 0;
			}
			if (cursor_and_keys.key_released_this_frame[PLAYER_GO_NEUTRAL_KEY] && player.can_go_neutral)
			{
				charge_map[entity_id].charge = player_map[entity_id].default_charge;
			}
			if (cursor_and_keys.key_pressed_this_frame[PLAYER_SWITCH_CHARGE_KEY] && player.can_switch_charge)
			{
				charge_map[entity_id].charge = -charge_map[entity_id].charge;
				player_map[entity_id].default_charge = -player_map[entity_id].default_charge;

				level.screen_size_shake_animation[globals.time] = 1;
				level.screen_size_shake_animation[globals.time + 0.05f] = 1.005;
				level.screen_size_shake_animation[globals.time + 0.1f] = 1;
			}
		}
	}
};