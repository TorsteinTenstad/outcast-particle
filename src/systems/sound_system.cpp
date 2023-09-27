#pragma once
#include "systems/sound_system.hpp"
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/SoundSource.hpp"
#include "components/sound_info.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include <iostream>

void SoundSystem::Update(Level& level, float dt)
{
	float max_volume = 100; //SFML uses 0-100 for volume, we use 0-1
	for (auto const& [entity, sound_info] : level.GetEntitiesWith<SoundInfo>())
	{
		if (!sound_info->play_sound.empty())
		{
			SoundTypes play_sound = sound_info->play_sound.front();
			assert(sound_info->sound_paths.count(play_sound) == 1);
			std::string sound_path = sound_info->sound_paths.at(play_sound);
			if (sound_buffers_.count(sound_path) == 0)
			{
				sound_buffers_[sound_path].loadFromFile(sound_path);
			}

			float volume_fraction = sound_info->sound_volumes.count(play_sound) == 1 ? sound_info->sound_volumes.at(play_sound) : 1;
			sounds_[sound_path].setBuffer(sound_buffers_[sound_path]);
			sounds_[sound_path].setVolume(max_volume * volume_fraction * globals.general_config.sound_volume / 100);
			sounds_[sound_path].play();
			sound_info->play_sound.pop();
		}
		for (auto [loop_sound, loop] : sound_info->loop_sounds)
		{
			assert(sound_info->sound_paths.count(loop_sound) == 1);
			sounds_[sound_info->sound_paths.at(loop_sound)].setLoop(true);
			sounds_[sound_info->sound_paths.at(loop_sound)].setVolume(max_volume * sound_info->sound_volumes.at(loop_sound) * globals.general_config.ambient_volume / 50);
		}
	}

	/*for (auto [sound_path, sound] : sounds_)
	{
		if (sound.getStatus() == sf::SoundSource::Status::Stopped)
		{
			sounds_.erase(sound_path);
		}
	}*/
}