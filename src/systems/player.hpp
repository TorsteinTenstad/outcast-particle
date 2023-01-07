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
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		if (mode_ != PLAY_MODE)
		{
			return;
		}
		for (const auto& [entity_id, player, player_behaviours, received_forces, charge, shader, sound_info, position] : level.GetEntitiesWith<Player, PlayerBehaviors, ReceivedForces, Charge, Shader, SoundInfo, Position>())
		{
			int x_direction = 0;
			if (cursor_and_keys_.key_down[globals.key_config.PLAYER_MOVE_LEFT])
			{
				x_direction -= 1;
			}
			if (cursor_and_keys_.key_down[globals.key_config.PLAYER_MOVE_RIGHT])
			{
				x_direction += 1;
			}
			int y_direction = 0;
			if (cursor_and_keys_.key_down[globals.key_config.PLAYER_MOVE_UP])
			{
				y_direction -= 1;
			}
			if (cursor_and_keys_.key_down[globals.key_config.PLAYER_MOVE_DOWN])
			{
				y_direction += 1;
			}
			received_forces->player_force.x = x_direction * player->move_force;
			received_forces->player_force.y = y_direction * player->move_force;

			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.PLAYER_GO_NEUTRAL] && player->can_go_neutral)
			{
				player_behaviours->default_charge = charge->charge;
				charge->charge = 0;
			}
			if (cursor_and_keys_.key_released_this_frame[globals.key_config.PLAYER_GO_NEUTRAL] && player->can_go_neutral)
			{
				charge->charge = player_behaviours->default_charge;
			}
			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.PLAYER_SWITCH_CHARGE] && player->can_switch_charge)
			{
				sound_info->play_sound = true;
				charge->charge = -charge->charge;
				player_behaviours->default_charge = -player_behaviours->default_charge;
				shader->float_uniforms["start_animation"] = globals.time;
			}
			shader->vec_uniforms["_position"] = position->position;
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};
