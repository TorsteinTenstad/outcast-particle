#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "general_user_config.hpp"
#include "level.hpp"

#include "sfml_event_handler.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "systems/coin.hpp"
#include "systems/draw.hpp"
#include "systems/level_completion_time.hpp"
#include "systems/level_menu.hpp"
#include "systems/menu_escape.hpp"
#include "systems/pause_mode.hpp"
#include "systems/render_grid_adaptive_textures.hpp"
#include "systems/render_shapes.hpp"
#include "systems/render_text.hpp"
#include "systems/render_trail.hpp"
#include "systems/sound_system.hpp"
#include <stack>

class Game
{
private:
	int physics_ticks_per_frame_ = 10;

	bool restart_update_loop_ = false;
	bool is_in_level_editing_ = false;

	std::stack<std::string> menu_stack;

	std::string active_level_id_ = MAIN_MENU;
	Level active_level_;
	std::map<std::string, std::vector<std::string>> level_groups_;
	std::map<int, std::map<std::string, float>> level_completion_time_records_;
	std::map<std::string, int> level_coin_records_;

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
	void ButtonFuncEditLevel();
	std::string GenerateLevelTexture(std::string level_id, unsigned width, unsigned height);
	void CheckFullscreen();
	void ToggleFullscreen();
	void CheckFramerateLimit();
	void ToggleFramerateLimit();
	void ExitGame();

	void GoToMainMenu();
	void GoToLevelMenu();
	void GoToOptionsMenu();
	void GoToKeyConfigMenu();
	void GoToGraphicsAndDisplayMenu();
	void GoToLastMenu();

public:
	Game();
	~Game();
	void Update(float dt);
};

#include "game.tpp"