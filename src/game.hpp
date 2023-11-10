#pragma once
#include "controls_config.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "general_user_config.hpp"
#include "level.hpp"
#include "level_manager.hpp"
#include "records_manager.hpp"
#include "server_transceiver.hpp"
#include "sfml_event_handler.hpp"

#include <memory>
#include <stack>

class Game
{
private:
	int physics_ticks_per_frame_ = 10;

	bool is_in_level_editing_ = false;

	std::stack<std::string> menu_stack;

	std::string active_level_id_ = MAIN_MENU;
	std::unique_ptr<Level> active_level_;

	RecordsManager records_;
	LevelManager level_manager_;

	int next_available_system_id_ = 0;
	std::map<std::type_index, int> type_to_system_id_;
	std::vector<int> game_system_entities_;
	std::vector<int> physics_game_system_entities_;
	std::map<int, std::unique_ptr<GameSystem>> game_systems_;

	sf::Clock absolute_clock;

	ServerTransceiver server_transceiver_;
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
	void ResetKeyConfig();
	void ExitGame();

	void GoToMainMenu();
	void GoToLevelMenu();
	void GoToOptionsMenu();
	void GoToCreditsMenu();
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