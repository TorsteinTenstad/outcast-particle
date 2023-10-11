
#include "music.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "entity_creation.hpp"
#include "globals.hpp"
#include <filesystem>
bool start = false;
void MusicSystem::Update(Level& level, float dt)
{
	if (!start)
	{
		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::S])
		{
			start = true;

			sf::Vector2f level_size = level.GetSize();
			float rx = level_size.x / 2.f;
			float ry = level_size.y / 2.f;
			Entity player_entity = level.AddBlueprint(BPPlayer);
			level.GetComponent<Player>(player_entity)->can_move_self = true;
			level.GetComponent<Player>(player_entity)->move_force = 300;
			level.GetComponent<Position>(player_entity)->position = sf::Vector2f(rx, 1.6 * ry);
			level.GetComponent<Velocity>(player_entity)->velocity = sf::Vector2f(46, 0);
			level.GetComponent<Charge>(player_entity)->charge *= -0.8;
		}
		return;
	}
	std::string music_path;
	if (is_in_level_editing_ && level.GetMode() != PLAY_MODE)
	{
		music_path = (DEFAULT_EDIT_MODE_SONG).string();
	}
	else
	{
		music_path = level.music_path;
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