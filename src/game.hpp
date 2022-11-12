#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "level.hpp"
#include "systems/acceleration.hpp"
#include "systems/display_velocity.hpp"
#include "systems/edit_mode.hpp"
#include "systems/electric_force.hpp"
#include "systems/force.hpp"
#include "systems/goal.hpp"
#include "systems/intersection.hpp"
#include "systems/kill_on_intersection.hpp"
#include "systems/level_button.hpp"
#include "systems/mouse_interactions.hpp"
#include "systems/player.hpp"
#include "systems/sfml_event.hpp"
#include "systems/sfml_render.hpp"
#include "systems/velocity.hpp"

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
	IntersectionSystem intersection_system_;
	SFMLRenderSystem render_system_;
	KillOnIntersectionSystem kill_on_intersection_system_;
	GoalSystem goal_system_;
	CursorAndKeys cursor_and_keys_;

public:
	Game();
	void Init();
	void Update(float dt);
};