
#include "music.hpp"

/*
store currently playing (plus bool?)
play music (bool for stop too?)
check if the level's music string is similar to its own, and change song accordingly
check for possible volume changes etc

*/

void MusicSystem::Update(Level& level, float dt)
{
	if (music_path_ != level.music_path)
	{
		music.openFromFile(level.music_path);
		music.play();
		music.setLoop(true);
		music_path_ = level.music_path;
	}
	music.setVolume(globals.general_config.music_volume);
}