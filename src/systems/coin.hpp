#pragma once
#include "game_system.hpp"
#include "level.hpp"
#include <string>

class CoinSystem : public GameSystem
{
private:
	std::map<std::string, int>* level_coin_records_;

public:
	using GameSystem::GameSystem;
	void SetCoinRecords(std::map<std::string, int>* coin_records);
	void Update(Level& level, float dt);
};