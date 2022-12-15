#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "level.hpp"
#include "modes.hpp"
#include "systems/acceleration.hpp"
#include "systems/button.hpp"
#include "systems/collision.hpp"
#include "systems/display_velocity.hpp"
#include "systems/draw.hpp"
#include "systems/edit_mode.hpp"
#include "systems/electric_field_force.hpp"
#include "systems/electric_force.hpp"
#include "systems/force.hpp"
#include "systems/goal.hpp"
#include "systems/intersection.hpp"
#include "systems/kill_on_intersection.hpp"
#include "systems/magnetic_field_force.hpp"
#include "systems/mode_system.hpp"
#include "systems/mouse_interactions.hpp"
#include "systems/pause_mode.hpp"
#include "systems/player.hpp"
#include "systems/render_shapes.hpp"
#include "systems/render_text.hpp"
#include "systems/render_trail.hpp"
#include "systems/screen_shake.hpp"
#include "systems/set_draw_info.hpp"
#include "systems/sfml_event.hpp"
#include "systems/sound_system.hpp"
#include "systems/trail.hpp"
#include "systems/velocity.hpp"

class Game
{
private:
	int physics_ticks_per_frame_ = 10;

	std::map<int, Level> levels_;
	int active_level_ = MAIN_MENU;
	Mode active_mode_ = PLAY_MODE;

	bool fullscreen_ = false;

	SFMLEventSystem event_system_;
	EditModeSystem edit_mode_system_;
	ButtonSystem button_system_;
	MouseInterationSystem mouse_interaction_system_;
	DisplayVelocitySystem display_velocity_system_;
	PlayerSystem player_system_;
	ElectricForceSystem electric_force_system_;
	ForceSystem force_system_;
	AccelerationSystem acceleration_system_;
	VelocitySystem velocity_system_;
	TrailSystem trail_system_;
	IntersectionSystem intersection_system_;
	SetDrawInfoSystem set_draw_info_system_;
	RenderShapesSystem render_shapes_system_;
	RenderTextSystem render_text_system_;
	RenderTrailSystem render_trail_system_;
	DrawSystem draw_system_;
	ScreenShakeSystem screen_shake_system_;
	KillOnIntersectionSystem kill_on_intersection_system_;
	GoalSystem goal_system_;
	CursorAndKeys cursor_and_keys_;
	CollisionSystem collision_system_;
	ElectricFieldForceSystem electric_field_force_system;
	MagneticFieldForceSystem magnetic_field_force_system;
	SoundSystem sound_system_;
	ModeSystem mode_system_;
	PauseMode pause_mode_;

	void UpdatePhysics(float dt);
	void SetLevel(int level);
	void ResetActiveLevel();
	int GetLevel();
	std::string GenerateLevelTexture(int level_id);
	void SetMode(Mode next_mode);
	Mode GetMode();
	void ToggleFullscreen();
	void ExitGame();

public:
	Game();
	~Game();
	void Init();
	void Update(float dt);
};