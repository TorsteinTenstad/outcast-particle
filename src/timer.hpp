#pragma once
#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> init_time_;
	std::chrono::time_point<std::chrono::high_resolution_clock> last_update_time_;

public:
	Timer();
	float GetElapsedSeconds();
	float GetSecondsSinceInit();
};
