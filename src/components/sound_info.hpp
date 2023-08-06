#pragma once
#include <map>
#include <queue>
#include <string>

enum SoundTypes
{
	DEFAULT,
	ON_CLICK,
	TO_NEUTRAL,
	FROM_NEUTRAL
};

class SoundInfo
{
public:
	std::map<SoundTypes, std::string> sound_paths;
	std::queue<SoundTypes> play_sound;
	std::map<SoundTypes, float> sound_volumes; // The scale is 0 - 1
	std::map<SoundTypes, bool> loop_sounds;
};