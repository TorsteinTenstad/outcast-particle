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
		auto& received_forces_map = level.GetComponent<ReceivedForces>();
		auto& velocity_map = level.GetComponent<Velocity>();
		auto& player_map = level.GetComponent<Player>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& player_behaviours_map = level.GetComponent<PlayerBehaviors>();
		auto& radius_map = level.GetComponent<Radius>();
		auto& charge_map = level.GetComponent<Charge>();
		auto& shader_map = level.GetComponent<Shader>();
		auto& sound_info_map = level.GetComponent<SoundInfo>();

		for (auto& [entity_id, player] : player_map)
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
			received_forces_map[entity_id].player_force.x = x_direction * player.move_force;
			received_forces_map[entity_id].player_force.y = y_direction * player.move_force;

			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.PLAYER_GO_NEUTRAL] && player.can_go_neutral)
			{
				player_behaviours_map[entity_id].default_charge = charge_map[entity_id].charge;
				charge_map[entity_id].charge = 0;
			}
			if (cursor_and_keys_.key_released_this_frame[globals.key_config.PLAYER_GO_NEUTRAL] && player.can_go_neutral)
			{
				charge_map[entity_id].charge = player_behaviours_map[entity_id].default_charge;
			}
			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.PLAYER_SWITCH_CHARGE] && player.can_switch_charge)
			{
				sound_info_map[entity_id].play_sound = true;
				charge_map[entity_id].charge = -charge_map[entity_id].charge;
				player_behaviours_map[entity_id].default_charge = -player_behaviours_map[entity_id].default_charge;
				assert(shader_map.count(entity_id) > 0);
				shader_map[entity_id].uniforms["time"] = 0;
			}
			shader_map[entity_id].uniforms["time"] += dt;
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};
