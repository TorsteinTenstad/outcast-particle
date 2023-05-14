#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "general_user_config.hpp"
#include "level.hpp"
#include "level_manager.hpp"
#include "sfml_event_handler.hpp"

#include <memory>
#include <stack>

class Game
{
private:
	int physics_ticks_per_frame_ = 10;

	bool restart_update_loop_ = false;
	bool is_in_level_editing_ = false;

	std::stack<std::string> menu_stack;

	std::string active_level_id_ = MAIN_MENU;
	MultiState<Level> active_level_;
	std::map<int, std::map<std::string, float>> level_completion_time_records_;
	std::map<std::string, int> level_coin_records_;

	LevelManager level_manager_;

	int next_available_system_id_ = 0;
	std::map<std::type_index, int> type_to_system_id_;
	std::vector<int> game_system_ids_;
	std::vector<int> physics_game_system_ids_;
	std::map<int, std::unique_ptr<GameSystem>> game_systems_;

	sf::Clock absolute_clock;

	SFMLEventHandler sfml_event_handler_;
	CursorAndKeys cursor_and_keys_;

	Level& GetActiveLevel();

	template <class System>
	System& GetGameSystem();

	template <class System>
	System& RegisterGameSystem(std::vector<int>& category);

	template <class System>
	System& RegisterGameSystem();

	template <class System>
	System& RegisterPhysicsGameSystem();

	Level& SetLevelAndEdit(std::string level_id);
	Level& SetLevel(std::string level_id);
	std::string GenerateLevelTexture(std::string level_id, unsigned width, unsigned height);
	void CheckFullscreen();
	void ToggleFullscreen();
	void ExitGame();

	void GoToMainMenu();
	void GoToLevelMenu();
	void GoToOptionsMenu();
	void GoToKeyConfigMenu();
	void GoToGraphicsAndDisplayMenu();
	void GoToMusicAndSoundMenu();
	void GoToLastMenu();

public:
	Game();
	~Game();
	void Update(float dt);
};

#include "game.tpp"