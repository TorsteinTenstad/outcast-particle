
#include "music.hpp"
#include "entity_creation.hpp"

void MusicSystem::Update(Level& level, float dt)
{
	std::string music_path;
	if (is_in_level_editing_ && level.GetMode() != PLAY_MODE)
	{
		music_path = "content\\music\\edit_mode.wav";
	}
	else
	{
		music_path = level.music_path;
	}

	if (music_path_ != music_path)
	{
		if (!music.openFromFile(music_path))
		{
			music.openFromFile("content\\music\\bliss.wav");
			CreateTextPopup(level, sf::Vector2f(4.5, 1) * float(BLOCK_SIZE), "Failed to load \n intended music", 3, true);
		}
		music.play();
		music.setLoop(true);
		music_path_ = music_path;
	}

	music.setVolume(globals.general_config.music_volume);
}