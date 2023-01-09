#include "animation.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "controls_config.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

static void TrySetFaceTexture(Level& level, Children* children, const std::string& image_path)
{

	std::vector<int>& child_ids = children->ids_owned_by_component[typeid(VelocityDependentDrawLayer)];
	if (child_ids.size() > 0)
	{
		int face = child_ids[0];
		level.GetComponent<DrawInfo>()[face].image_path = image_path;
	}
}

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

		for (const auto& [entity_id, player, player_behaviours, received_forces, charge, children, shader, sound_info, position] : level.GetEntitiesWith<Player, PlayerBehaviors, ReceivedForces, Charge, Children, Shader, SoundInfo, Position>())
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
				shader->float_uniforms["start_switch_charge_animation"] = globals.time;
			}
			if (level.GetComponent<ScheduledDelete>().count(entity_id) > 0)
			{
				level.GetComponent<Shader>()[entity_id].float_uniforms["start_death_animation"] = globals.time;
				level.GetComponent<Intersection>().erase(entity_id);
				level.GetComponent<Velocity>().erase(entity_id);
				level.GetComponent<Player>().erase(entity_id);
				TrySetFaceTexture(level, children, "content\\textures\\face_dead.png");
			}
		}

		for (auto const& [entity_id, player, player_behaviors, charge, draw_info, children, velocity_dependent_draw_layer] : level.GetEntitiesWith<Player, PlayerBehaviors, Charge, DrawInfo, Children, VelocityDependentDrawLayer>())
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
			std::string face_path = "content\\textures\\face_" + std::string((nonzero_charge > 0) ? "plus.png" : "minus.png");
			TrySetFaceTexture(level, children, face_path);
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};