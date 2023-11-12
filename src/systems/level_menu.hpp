#pragma once
#include "components/level_menu.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "level_manager.hpp"
#include "records_manager.hpp"
#include "server_transceiver.hpp"
#include "utils/string_parsing.hpp"

#define LEVEL_PREVIEW_SCALE 0.6

class LevelMenuSystem : public GameSystem
{
private:
	LevelManager* level_manager_;
	const RecordsManager* records_;
	const ServerTransceiver* server_transceiver_;

	std::function<Level&(std::string)> set_level_;
	std::function<Level&(std::string)> set_level_and_edit_;
	std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture_;

public:
	using GameSystem::GameSystem;
	void Give(
		LevelManager* level_manager,
		const RecordsManager* records,
		const ServerTransceiver* server_transceiver,
		std::function<Level&(std::string)> set_level,
		std::function<Level&(std::string)> set_level_and_edit_,
		std::function<std::string(std::string, unsigned, unsigned)> generate_level_texture);
	void Update(Level& level, float dt);

private:
	void UpdateUI(Level& level, LevelMenuUI* ui);
	void SetupUI(Level& level, LevelMenuUI* ui);
};