#pragma once
#include "globals.hpp"
#include <map>
#include <queue>
#include <string>

class SoundInfo
{
public:
	std::map<SoundTypes, std::string> sound_paths;
	std::queue<SoundTypes> play_sound;
	std::map<SoundTypes, float> sound_volumes; // The scale is 0 - 1
	std::map<SoundTypes, bool> loop_sounds;
};