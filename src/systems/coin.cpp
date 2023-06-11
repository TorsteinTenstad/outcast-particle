#include "components/coin.hpp"
#include "components/intersection.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/scheduled_delete.hpp"
#include "components/screen_wide_shader_effects.hpp"
#include "components/sound_info.hpp"
#include "globals.hpp"
#include "systems/_pure_DO_systems.hpp"
#include <cassert>

void CoinSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != PLAY_MODE)
	{
		return;
	}

	CoinCounter* coin_counter = level.GetSingleton<CoinCounter>();
	int& counter = coin_counter->coin_counter;

	for (auto& [entity, player, intersection] : level.GetEntitiesWith<Player, Intersection>())
	{
		for (auto& entity : intersection->entities_entered_this_frame)
		{
			if (!level.HasComponents<Coin>(entity)) { continue; }
			assert(level.HasComponents<SegmentedGlowEffect>(entity));
			level.RemoveComponents<Coin>(entity);
			level.GetComponent<SegmentedGlowEffect>(entity)->animation_start_time = globals.time;
			level.GetComponent<SoundInfo>(entity)->play_sound.push(DEFAULT);
			level.AddComponent<ScheduledDelete>(entity)->delete_at = globals.time + 1;
			counter += 1;
		}
	}
}