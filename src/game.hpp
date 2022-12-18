#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "level.hpp"
#include "modes.hpp"
#include "sfml_event_handler.hpp"
#include "systems/acceleration.hpp"
#include "systems/background.hpp"
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
#include "systems/level_button.hpp"
#include "systems/level_completion_time.hpp"
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
#include "systems/sound_system.hpp"
#include "systems/trail.hpp"
#include "systems/velocity.hpp"

class Game
{
private:
	int physics_ticks_per_frame_ = 10;
	bool fullscreen_ = false;

	int next_available_level_id_ = 0;
	std::map<int, Level> levels_;
	std::map<int, float> level_completion_time_records_;

	int active_level_ = MAIN_MENU;
	Mode active_mode_ = PLAY_MODE;

	int next_available_system_id_ = 0;
	std::map<std::type_index, int> system_ids_;
	std::map<int, std::unique_ptr<GameSystem>> game_systems_;
	std::map<int, std::unique_ptr<GameSystem>> physics_game_systems_;

	SFMLEventHandler sfml_event_handler_;
	CursorAndKeys cursor_and_keys_;

	template <class System>
	System& GetGameSystem();

	template <class System>
	System& RegisterGameSystem(std::map<int, std::unique_ptr<GameSystem>>& map);

	template <class System>
	System& RegisterGameSystem();

	template <class System>
	System& RegisterPhysicsGameSystem();

	Level& AddLevel();
	Level& AddLevel(int id);
	Level& GetLevel(int id);
	void SetLevel(int level);
	void ResetActiveLevel();
	std::string GenerateLevelTexture(int level_id);
	void SetMode(Mode next_mode);
	Mode GetMode();
	void ToggleFullscreen();
	void ExitGame();
	bool InLevel();

public:
	Game();
	~Game();
	void Init();
	void Update(float dt);
};

#include "game.tpp"