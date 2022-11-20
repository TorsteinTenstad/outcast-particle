#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "level.hpp"
#include "systems/acceleration.hpp"
#include "systems/collision.hpp"
#include "systems/display_velocity.hpp"
#include "systems/edit_mode.hpp"
#include "systems/electric_field_force.hpp"
#include "systems/electric_force.hpp"
#include "systems/force.hpp"
#include "systems/goal.hpp"
#include "systems/intersection.hpp"
#include "systems/kill_on_intersection.hpp"
#include "systems/level_button.hpp"
#include "systems/magnetic_field_force.hpp"
#include "systems/mouse_interactions.hpp"
#include "systems/player.hpp"
#include "systems/set_draw_info.hpp"
#include "systems/sfml_event.hpp"
#include "systems/sfml_render.hpp"
#include "systems/velocity.hpp"

class Game
{
private:
	int physics_ticks_per_frame_ = 10;
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
	SetDrawInfoSystem set_draw_info_system_;
	SFMLRenderSystem render_system_;
	KillOnIntersectionSystem kill_on_intersection_system_;
	GoalSystem goal_system_;
	CursorAndKeys cursor_and_keys_;
	CollisionSystem collision_system_;
	ElectricFieldForceSystem electric_field_force_system;
	MagneticFieldForceSystem magnetic_field_force_system;

public:
	Game();
	~Game();
	void Init();
	void Update(float dt);
	void UpdatePhysics(float dt);
};