#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "level.hpp"

#include "sfml_event_handler.hpp"
#include "systems/acceleration.hpp"
#include "systems/animated_position.hpp"
#include "systems/background.hpp"
#include "systems/button.hpp"
#include "systems/button_events.hpp"
#include "systems/coin.hpp"
#include "systems/collision.hpp"
#include "systems/display_velocity.hpp"
#include "systems/draw.hpp"
#include "systems/edit_mode.hpp"
#include "systems/edit_mode_ui.hpp"
#include "systems/electric_field_force.hpp"
#include "systems/electric_force.hpp"
#include "systems/face.hpp"
#include "systems/force.hpp"
#include "systems/force_visualization.hpp"
#include "systems/goal.hpp"
#include "systems/grid_position.hpp"
#include "systems/intersection.hpp"
#include "systems/kill_on_intersection.hpp"
#include "systems/level_completion_time.hpp"
#include "systems/level_menu.hpp"
#include "systems/magnetic_field_force.hpp"
#include "systems/menu_navigator.hpp"
#include "systems/pause_mode.hpp"
#include "systems/player.hpp"
#include "systems/render_shapes.hpp"
#include "systems/render_text.hpp"
#include "systems/render_trail.hpp"
#include "systems/scheduled_delete.hpp"
#include "systems/segmented_glow_effect.hpp"
#include "systems/set_draw_info.hpp"
#include "systems/sound_system.hpp"
#include "systems/sticky_button.hpp"
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

	std::string active_level_id_ = MAIN_MENU;
	Level active_level_;
	std::map<std::string, std::vector<std::string>> level_groups_;
	std::map<int, std::map<std::string, float>> level_completion_time_records_;
	std::map<std::string, int> level_coin_records_;
	std::vector<std::string> user_savefile_paths = { "user\\Completion_Time_0_Coins.txt", "user\\Completion_Time_1_Coins.txt", "user\\Completion_Time_2_Coins.txt", "user\\Completion_Time_3_Coins.txt" };

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

	Level& SetLevel(std::string level_id);
	void ButtunFuncEditLevel();
	std::string GenerateLevelTexture(std::string level_id, unsigned width, unsigned height);
	void ToggleFullscreen();
	void ExitGame();

	void GoToMainMenu();
	void GoToLevelMenu();
	void GoToOptionsMenu();

public:
	Game();
	~Game();
	void Update(float dt);
};

#include "game.tpp"