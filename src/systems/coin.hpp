#pragma once
#include "components/coin.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "utils.hpp"
#include <iostream>

class CoinSystem : public GameSystem
{
private:
	std::map<int, int>* coin_records_;

public:
	using GameSystem::GameSystem;
	void SetCoinRecords(std::map<int, int>* coin_records)
	{
		coin_records_ = coin_records;
	}
	void Update(Level& level, float dt)
	{
		auto& intersection_map = level.GetComponent<Intersection>();
		auto& coin_map = level.GetComponent<Coin>();
		auto& sound_info_map = level.GetComponent<SoundInfo>();

		CoinCounter* coin_counter = GetSingleton<CoinCounter>(level);
		int counter = coin_counter->coin_counter;

		for (auto& [entity_id, intersection] : level.GetEntitiesWith<Intersection>())
		{
			for (auto& i : intersection->entered_this_frame_ids)
			{
				if (coin_map.count(i) != 0)
				{
					sound_info_map[i].play_sound = true;
					level.GetComponent<ScheduledDelete>()[i].delete_at = globals.time;
					counter += 1;
				}
			}
		}

		if (mode_ == LEVEL_COMPLETED_MODE && ((*coin_records_)[level.id] <= 0 || (*coin_records_)[level.id] > counter))
		{
			(*coin_records_)[level.id] = counter;
			return;
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};