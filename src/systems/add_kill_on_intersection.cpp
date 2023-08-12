#pragma once
#include "Components/laser.hpp"
#include "Components/physics.hpp"
#include "Components/size.hpp"
#include "Components/sound_info.hpp"
#include "_pure_DO_systems.hpp"
#include "entity_creation.hpp"
#include <iostream>

void AddKillOnIntersectionSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != PLAY_MODE) { return; }
	for (auto [laser_id, laser, width_and_height, position] : level.GetEntitiesWith<Laser, WidthAndHeight, Position>())
	{
		if (laser->kill_on_intersection_enabled)
		{
			Entity child = GetSingletonChildId<Laser>(level, laser_id, [](Level& level) {
				auto [entity, kill_on_intersection, position, width_and_height] = level.CreateEntityWith<KillOnIntersection, Position, WidthAndHeight>();
				level.AddComponent<SoundInfo>(entity)->sound_paths = { { DEFAULT, "content\\sounds\\laser.wav" } };
				return entity;
			});
			level.GetComponent<WidthAndHeight>(child)->width_and_height = width_and_height->width_and_height - sf::Vector2f(40, 40);
			level.GetComponent<Position>(child)->position = position->position;
		}
		else
		{
			DeleteChildrenOwnedBy<Laser>(level, laser_id);
		}
	}
}
