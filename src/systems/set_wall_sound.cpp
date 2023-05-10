#pragma once
#include "Components/collision.hpp"
#include "Components/sound_info.hpp"
#include "constants.hpp"
#include "grid_adaptive_textures.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/container_operations.hpp"

void SetWallSoundSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, wall, collision, sound_info] : level.GetEntitiesWith<Wall, Collision, SoundInfo>())
	{
		unsigned sound_index = FindClosest(WALL_BOUNCE_CATEGORIES, collision->bounce_factor);
		sound_info->sound_paths[DEFAULT] = WALL_SOUND_CATEGORIES[sound_index];
	}
}