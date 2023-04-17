#pragma once
#include <string>

class SoundInfo
{
public:
	std::string sound_path;
	bool play_sound = false;
	float sound_volume = 1; // The scale is 0 - 1, default is 1
	bool loop_sound = false;
};