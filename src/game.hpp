#pragma once
#include "cursor_and_keys.hpp"
#include "level.hpp"
#include "sys_acceleration.hpp"
#include "sys_display_velocity.hpp"
#include "sys_edit_mode.hpp"
#include "sys_electric_force.hpp"
#include "sys_force.hpp"
#include "sys_level_button.hpp"
#include "sys_mouse_interactions.hpp"
#include "sys_player.hpp"
#include "sys_sfml_event.hpp"
#include "sys_sfml_render.hpp"
#include "sys_velocity.hpp"

class Game
{
private:
	std::vector<Level> levels_;

	SFMLEventSystem event_system_;
	EditModeSystem edit_mode_system_;
	LevelButtonSystem level_button_system_;
	MouseInterationSystem mouse_interaction_system_;
	DisplayVelocitySystem display_velocity_system_;
	PlayerSystem player_system_;
	ElectricForceSystem electric_force_system_;
	ForceSystem force_system_;
	AccelerationSystem acceleration_system_;
	VelocitySystem velocity_system_;
	SFMLRenderSystem render_system_;

	CursorAndKeys cursor_and_keys_;

public:
	Game();
	void Update(float dt);
};