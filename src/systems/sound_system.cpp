#pragma once
#include "systems/sound_system.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/SoundSource.hpp"
#include "components/sound_info.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

void SoundSystem::Update(Level& level, float dt)
{
	for (auto const& [entity_id, sound_info] : level.GetEntitiesWith<SoundInfo>())
	{
		std::string sound_path = sound_info->sound_path;
		if (sound_buffers_.count(sound_path) == 0)
		{
			sound_buffers_[sound_path].loadFromFile(sound_path);
		}
		float max_volume = 100; //SFML uses 0-100 for volume, we use 0-1

		if (sound_info->play_sound)
		{

			if (MAX_VOLUME.count(sound_path) > 0)
			{
				max_volume = MAX_VOLUME[sound_path];
			}
			sounds_[sound_path].setBuffer(sound_buffers_[sound_path]);
			sounds_[sound_path].setVolume(max_volume * sound_info->sound_volume * globals.general_config.sound_volume / 100);
			sounds_[sound_path].play();
			sound_info->play_sound = false;
		}
		if (sound_info->loop_sound)
		{
			sounds_[sound_path].setLoop(true);
			sounds_[sound_path].setVolume(max_volume * sound_info->sound_volume * globals.general_config.sound_volume / 100);
		}

		if (sounds_.count(sound_path) != 0)
		{
			if (sounds_[sound_path].getStatus() == sf::SoundSource::Status::Stopped)
			{
				sounds_.erase(sound_path);
			}
		}
	}
}