#pragma once
#include "systems/_pure_DO_systems.hpp"
#include "utils/container_operations.hpp"
#include "utils/math.hpp"
#include <algorithm>
#include <iostream>

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
	if (level.GetMode() != PLAY_MODE && level.ComputeState() == PLAYING)
	{
		return;
	}

	float smallest_laser_distance = 1000;
	for (auto [player_id, player, player_position] : level.GetEntitiesWith<Player, Position>())
	{
		for (auto [laser_id, kill_on_intersection, laser_position, laser_width_and_height] : level.GetEntitiesWith<KillOnIntersection, Position, WidthAndHeight>())
		{
			float min_distance = FindShortestDistance(player_position->position, laser_position->position, laser_width_and_height->width_and_height);
			if (min_distance < smallest_laser_distance)
			{
				smallest_laser_distance = min_distance;
				//std::cout << laser_id << std::endl;
			}
		}
	}
	//std::cout << smallest_laser_distance << std::endl;
}