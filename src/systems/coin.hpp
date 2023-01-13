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

		int& coin_counter = EnsureExistanceOfCoinCounter(level);

		for (auto& [entity_id, intersection] : level.GetEntitiesWith<Intersection>())
		{
			for (auto& i : intersection->entered_this_frame_ids)
			{
				if (coin_map.count(i) != 0)
				{
					sound_info_map[i].play_sound = true;
					level.GetComponent<ScheduledDelete>()[i].delete_at = globals.time;
					coin_counter += 1;
				}
			}
		}

		if (mode_ == LEVEL_COMPLETED_MODE && ((*coin_records_)[level.id] <= 0 || (*coin_records_)[level.id] > coin_counter))
		{
			(*coin_records_)[level.id] = coin_counter;
			return;
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
	int& EnsureExistanceOfCoinCounter(Level& level)
	{
		auto& coin_counter_map = level.GetComponent<CoinCounter>();
		assert(!(coin_counter_map.size() > 1));

		if (coin_counter_map.size() == 0)
		{
			int entity_id = level.CreateEntityId();
			coin_counter_map[entity_id].coin_counter = 0;
		}
		return coin_counter_map.begin()->second.coin_counter;
	}
};