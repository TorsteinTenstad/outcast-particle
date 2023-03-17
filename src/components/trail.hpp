#pragma once
#include "SFML/System/Vector2.hpp"
#include <vector>
#define TRAIL_N 64

class Trail
{
public:
	const int max_segments = 64;
	const float trail_length_in_seconds = 1;

	int segments_created = 0;
	int segment_to_update_next = 0;
	float seconds_until_next_update = 0;
};