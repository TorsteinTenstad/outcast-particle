
#include "music.hpp"
#include "entity_creation.hpp"
#include "globals.hpp"
#include <filesystem>

void MusicSystem::Update(Level& level, float dt)
{
	std::string music_path;
	if (is_in_level_editing_ && level.GetMode() != PLAY_MODE)
	{
		music_path = (MUSIC_DIR / "Edit Mode.wav").string();
	}
	else
	{
		music_path = level.music_path;
	}

	if (music_path_ != music_path)
	{
		if (!music.openFromFile(music_path))
		{
			music.openFromFile((MUSIC_DIR / "Bliss.wav").string());
			globals.errors += Error(ErrorNumber::LOAD_WAV, "Failed to load music from\n" + std::filesystem::absolute(std::filesystem::path(music_path)).string());
		}
		music.play();
		music.setLoop(true);
		music_path_ = music_path;
	}

	music.setVolume(globals.general_config.music_volume);
}