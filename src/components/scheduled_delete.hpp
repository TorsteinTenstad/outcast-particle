#pragma once

class ScheduledDelete
{
public:
	std::optional<float> delete_at;
	std::optional<int> frames_left_to_live;
};