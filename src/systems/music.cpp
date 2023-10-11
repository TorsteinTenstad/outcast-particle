
#include "music.hpp"
#include "components/select_music_button.hpp"
#include "components/sticky_button.hpp"
#include "entity_creation.hpp"
#include "globals.hpp"
#include <filesystem>

void MusicSystem::Update(Level& level, float dt)
{
	std::string music_path;
	if (is_in_level_editing_ && level.GetMode() != PLAY_MODE)
	{
		music_path = (DEFAULT_EDIT_MODE_SONG).string();
	}
	else
	{
		music_path = level.music_path;
	}
	for (auto [entity, select_music_button, sticky_button_down] : level.GetEntitiesWith<SelectMusicButton, StickyButtonDown>())
	{
		music_path = select_music_button->music_path;
	}
	if (music_path_ != music_path)
	{
		if (!music.openFromFile(music_path))
		{
			music.openFromFile(DEFAULT_MENU_SONG.string());
			globals.errors += Error(ErrorNumber::LOAD_WAV, "Failed to load music from\n" + std::filesystem::absolute(std::filesystem::path(music_path)).string());
		}
		music.play();
		music.setLoop(true);
		music_path_ = music_path;
	}

	music.setVolume(globals.general_config.music_volume);
}