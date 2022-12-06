#include "animation.hpp"
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
		std::map<int, PlayerBehaviours>& player_behaviours_map = level.GetComponent<PlayerBehaviours>();
		std::map<int, Radius>& radius_map = level.GetComponent<Radius>();
		std::map<int, Charge>& charge_map = level.GetComponent<Charge>();

		for (auto& [entity_id, player] : player_map)
		{
			int x_direction = 0;
			if (cursor_and_keys.key_down[globals.key_config.PLAYER_MOVE_LEFT])
			{
				x_direction -= 1;
			}
			if (cursor_and_keys.key_down[globals.key_config.PLAYER_MOVE_RIGHT])
			{
				x_direction += 1;
			}
			int y_direction = 0;
			if (cursor_and_keys.key_down[globals.key_config.PLAYER_MOVE_UP])
			{
				y_direction -= 1;
			}
			if (cursor_and_keys.key_down[globals.key_config.PLAYER_MOVE_DOWN])
			{
				y_direction += 1;
			}
			received_forces_map[entity_id].player_force.x = x_direction * player.move_force;
			received_forces_map[entity_id].player_force.y = y_direction * player.move_force;

			if (cursor_and_keys.key_pressed_this_frame[globals.key_config.PLAYER_GO_NEUTRAL] && player.can_go_neutral)
			{
				player_behaviours_map[entity_id].default_charge = charge_map[entity_id].charge;
				charge_map[entity_id].charge = 0;
			}
			if (cursor_and_keys.key_released_this_frame[globals.key_config.PLAYER_GO_NEUTRAL] && player.can_go_neutral)
			{
				charge_map[entity_id].charge = player_behaviours_map[entity_id].default_charge;
			}
			if (cursor_and_keys.key_pressed_this_frame[globals.key_config.PLAYER_SWITCH_CHARGE] && player.can_switch_charge)
			{
				charge_map[entity_id].charge = -charge_map[entity_id].charge;
				player_behaviours_map[entity_id].default_charge = -player_behaviours_map[entity_id].default_charge;

				level.screen_size_shake_animation[globals.time] = 1;
				//level.screen_size_shake_animation[globals.time + 0.05f] = 1.005;
				level.screen_size_shake_animation[globals.time + 0.1f] = 1;

				if (player_behaviours_map[entity_id].radius_animation.size() == 0)
				{
					player_behaviours_map[entity_id].default_radius = radius_map[entity_id].radius;
				}
				player_behaviours_map[entity_id].radius_animation[globals.time] = 1;
				player_behaviours_map[entity_id].radius_animation[globals.time + 0.05f] = 0.9;
				player_behaviours_map[entity_id].radius_animation[globals.time + 0.12f] = 1;
			}
			if (player_behaviours_map[entity_id].radius_animation.size() != 0)
			{
				Animate(globals.time, player_behaviours_map[entity_id].radius_modifier, player_behaviours_map[entity_id].radius_animation, FakeSigmoid);
				radius_map[entity_id].radius = player_behaviours_map[entity_id].default_radius * player_behaviours_map[entity_id].radius_modifier;
			}
		}
	}
};