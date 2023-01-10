#pragma once
#include <vector>

class Intersection
{
public:
	std::vector<int> intersecting_ids;
	std::vector<int> entered_this_frame_ids;
	std::vector<int> left_this_frame_ids;
};