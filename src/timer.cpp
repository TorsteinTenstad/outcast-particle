#include "timer.hpp"

Timer::Timer()
{
	last_update_time_ = std::chrono::high_resolution_clock::now();
	init_time_ = std::chrono::high_resolution_clock::now();
}

float Timer::GetElapsedSeconds()
{
	auto current_time = std::chrono::high_resolution_clock::now();
	float dt = (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - last_update_time_).count())
		/ 1'000'000'000;
	last_update_time_ = current_time;
	return dt;
}

float Timer::GetSecondsSinceInit()
{
	auto current_time = std::chrono::high_resolution_clock::now();
	return (float)(std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - init_time_).count())
		/ 1'000'000'000;
}
