#pragma once
#include "PCH.hpp"

class SoundInfo
{
public:
	std::string sound_path;
	bool play_sound = false;
	float sound_volume = 100; //the scale is 0 - 100, default is 100.
};