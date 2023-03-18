#pragma once
#include "SFML/System/Vector2.hpp"
#include <vector>

class Trail
{
public:
	const int max_segments = 64;

	int segments_created = 0;
	int segment_to_update_next = 0;
};