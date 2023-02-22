#include "components/player.hpp"
#include "_pure_DO_systems.hpp"
#include "components/physics.hpp"
#include "controls_config.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"

void ProcessPlayerControls(Level& level, CursorAndKeys& cursor_and_keys)
{
	int player_i = 0;
	for (const auto& [entity_id, player, player_behaviours, received_forces, charge, children, shader, sound_info, position] : level.GetEntitiesWith<Player, PlayerBehaviors, ReceivedForces, Charge, Children, Shader, SoundInfo, Position>())
	{
		int x_direction = 0;
		x_direction -= cursor_and_keys.key_down[globals.key_config.PLAYER_MOVE_LEFT] ? 1 : 0;
		x_direction += cursor_and_keys.key_down[globals.key_config.PLAYER_MOVE_RIGHT] ? 1 : 0;
		int y_direction = 0;
		y_direction -= cursor_and_keys.key_down[globals.key_config.PLAYER_MOVE_UP] ? 1 : 0;
		y_direction += cursor_and_keys.key_down[globals.key_config.PLAYER_MOVE_DOWN] ? 1 : 0;
		received_forces->player_force.x = x_direction * player->move_force;
		received_forces->player_force.y = y_direction * player->move_force;

		auto switch_key = player_i == 1 ? sf::Keyboard::C : globals.key_config.PLAYER_SWITCH_CHARGE;
		auto neutral_key = player_i == 1 ? sf::Keyboard::V : globals.key_config.PLAYER_GO_NEUTRAL;

		if (cursor_and_keys.key_pressed_this_frame[neutral_key] && player->can_go_neutral)
		{
			player_behaviours->default_charge = charge->charge;
			player_behaviours->is_neutral = true;
			charge->charge = 0;
		}
		if (player_behaviours->is_neutral && !cursor_and_keys.key_down[neutral_key])
		{
			charge->charge = player_behaviours->default_charge;
			player_behaviours->is_neutral = false;
		}
		if (cursor_and_keys.key_pressed_this_frame[switch_key] && player->can_switch_charge)
		{
			sound_info->play_sound = true;
			charge->charge = -charge->charge;
			player_behaviours->default_charge = -player_behaviours->default_charge;
			shader->float_uniforms["start_switch_charge_animation"] = globals.time;
		}
		player_i++;
	}
}

void SetTextures(Level& level)
{
	for (auto const& [entity_id, player, player_behaviors, charge, draw_info, children, face] : level.GetEntitiesWith<Player, PlayerBehaviors, Charge, DrawInfo, Children, Face>())
	{
		float nonzero_charge = 0;
		if (charge->charge != 0)
		{
			nonzero_charge = charge->charge;
			int category = FindClosest(PARTICLE_CHARGE_CATEGORIES, nonzero_charge);
			draw_info->image_path = PLAYER_PARTICLE_TEXTURES[category];
		}
		else
		{
			nonzero_charge = player_behaviors->default_charge;
			int category = FindClosest(PARTICLE_CHARGE_CATEGORIES, nonzero_charge);
			draw_info->image_path = PLAYER_PARTICLE_NEUTRAL_TEXTURES[category];
		}
		face->image_path = "content\\textures\\face_" + std::string((nonzero_charge > 0) ? "plus.png" : "minus.png");
	}
}

void PlayerSystem::Update(Level& level, float dt)
{
	if (level.GetMode() == PLAY_MODE)
	{
		ProcessPlayerControls(level, cursor_and_keys_);
	}
	SetTextures(level);
}