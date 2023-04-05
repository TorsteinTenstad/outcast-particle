#include "components/coin.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "globals.hpp"
#include "systems/coin.hpp"
#include <cassert>

void CoinSystem::SetCoinRecords(std::map<std::string, int>* coin_records)
{
	level_coin_records_ = coin_records;
}
void CoinSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != PLAY_MODE || in_edit_mode_)
	{
		return;
	}

	CoinCounter* coin_counter = level.GetSingleton<CoinCounter>();
	int& counter = coin_counter->coin_counter;

	for (auto& [entity_id, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
	{
		for (auto& id : intersection->entered_this_frame_ids)
		{
			if (!level.HasComponents<Coin>(id))
			{
				continue;
			}
			assert(level.HasComponents<Coin>(id));
			assert(level.HasComponents<SegmentedGlowEffect>(id));
			level.RemoveComponents<Coin>(id);
			level.GetComponent<SegmentedGlowEffect>(id)->animation_start_time = globals.time;
			level.GetComponent<SoundInfo>(id)->play_sound = true;
			level.AddComponent<ScheduledDelete>(id)->delete_at = globals.time + 1;
			counter += 1;
		}
	}

	if (level.ComputeState() == COMPLETED && counter > (*level_coin_records_)[active_level_id_])
	{
		(*level_coin_records_)[active_level_id_] = counter;
		return;
	}
}