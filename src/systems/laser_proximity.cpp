#pragma once
#include "components/kill_on_intersection.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "components/sound_info.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/container_operations.hpp"
#include "utils/math.hpp"
#include <algorithm>
#include <iostream>

class LaserProximity
{};

float FindShortestDistance(sf::Vector2f player_position, sf::Vector2f laser_position, sf::Vector2f laser_width_height)
{
	float abs_x_dist = abs(laser_position.x - player_position.x) - laser_width_height.x / 2.f;
	float abs_y_dist = abs(laser_position.y - player_position.y) - laser_width_height.y / 2.f;
	if (abs_x_dist < 0)
	{
		return (abs_y_dist);
	}
	else if (abs_y_dist < 0)
	{
		return (abs_x_dist);
	}
	else
	{
		return Magnitude(sf::Vector2f(abs_x_dist, abs_y_dist));
	}
}

void LaserProximitySystem::Update(Level& level, float dt)
{
	if (!globals.general_config.play_ambient_sounds)
	{
		return;
	}

	float minimum_laser_distance = 750;
	float smallest_laser_distance = minimum_laser_distance;
	for (auto [player_id, player, player_position] : level.GetEntitiesWith<Player, Position>())
	{
		for (auto [laser_id, kill_on_intersection, laser_position, laser_width_and_height] : level.GetEntitiesWith<KillOnIntersection, Position, WidthAndHeight>())
		{
			float min_distance = FindShortestDistance(player_position->position, laser_position->position, laser_width_and_height->width_and_height);
			if (min_distance < smallest_laser_distance)
			{
				smallest_laser_distance = min_distance;
			}
		}
	}
	int id = level.GetSingletonId<LaserProximity>([](ECSScene& level) {
		auto [id, sound_info] = level.CreateEntityWith<SoundInfo>();
		sound_info->sound_path = "content\\sounds\\laser_proximity.wav";
		return id;
	});

	auto sound_info = level.GetComponent<SoundInfo>(id);
	if (smallest_laser_distance >= minimum_laser_distance || (level.GetMode() != PLAY_MODE))
	{
		sound_info->loop_sound = false;
		return;
	}
	else if (!sound_info->loop_sound)
	{
		sound_info->play_sound = true;
		sound_info->loop_sound = true;
	}
	float volume = std::min(1.f, 750000 / (smallest_laser_distance * smallest_laser_distance * smallest_laser_distance));
	std::cout << volume * 100 << std::endl;
	sound_info->sound_volume = volume;
}