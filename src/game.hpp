#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "level.hpp"

#include "sfml_event_handler.hpp"
#include "systems/acceleration.hpp"
#include "systems/animated_position.hpp"
#include "systems/background.hpp"
#include "systems/button.hpp"
#include "systems/coin.hpp"
#include "systems/collision.hpp"
#include "systems/display_velocity.hpp"
#include "systems/draw.hpp"
#include "systems/edit_mode.hpp"
#include "systems/electric_field_force.hpp"
#include "systems/electric_force.hpp"
#include "systems/face.hpp"
#include "systems/force.hpp"
#include "systems/force_visualization.hpp"
#include "systems/goal.hpp"
#include "systems/intersection.hpp"
#include "systems/kill_on_intersection.hpp"
#include "systems/level_button.hpp"
#include "systems/level_completion_time.hpp"
#include "systems/magnetic_field_force.hpp"
#include "systems/menu_navigator.hpp"
#include "systems/mouse_interactions.hpp"
#include "systems/pause_mode.hpp"
#include "systems/player.hpp"
#include "systems/render_shapes.hpp"
#include "systems/render_text.hpp"
#include "systems/render_trail.hpp"
#include "systems/scheduled_delete.hpp"
#include "systems/segmented_glow_effect.hpp"
#include "systems/set_draw_info.hpp"
#include "systems/sound_system.hpp"
#include "systems/text_popup.hpp"
#include "systems/trail.hpp"
#include "systems/velocity.hpp"
#include "systems/view.hpp"

class Game
{
private:
	int physics_ticks_per_frame_ = 10;
	bool fullscreen_ = false;

	bool restart_update_loop_ = false;
	bool is_in_level_editing_ = false;

	int next_available_level_id_ = 0;
	int active_level_id_ = MAIN_MENU;
	Level active_level_;
	std::map<std::string, std::vector<int>> level_groups_;
	std::map<int, std::string> level_paths_;
	std::map<int, float> level_completion_time_records_;
	std::map<int, int> level_coin_records_;

	int next_available_system_id_ = 0;
	std::map<std::type_index, int> type_to_system_id_;
	std::vector<int> game_system_ids_;
	std::vector<int> physics_game_system_ids_;
	std::map<int, std::unique_ptr<GameSystem>> game_systems_;

	SFMLEventHandler sfml_event_handler_;
	CursorAndKeys cursor_and_keys_;

	template <class System>
	System& GetGameSystem();

	template <class System>
	System& RegisterGameSystem(std::vector<int>& category);

	template <class System>
	System& RegisterGameSystem();

	template <class System>
	System& RegisterPhysicsGameSystem();

	void SetLevel(int level_id);
	void ButtunFuncEditLevel();
	std::string GenerateLevelTexture(int level_id);
	void ToggleFullscreen();
	void ExitGame();

	void GoToMainMenu();
	void GoToLevelMenu();
	void GoToOptionsMenu();

public:
	Game();
	void Update(float dt);
};

#include "game.tpp"