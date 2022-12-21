#pragma once
#include "Components/sound_info.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class SoundSystem : public GameSystem
{
private:
	std::map<std::string, float> MAX_VOLUME = { { "content\\sounds\\laser.wav", 20 } };
	std::map<std::string, sf::SoundBuffer> sound_buffers_;
	std::map<std::string, sf::Sound> sounds_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& sound_info_map = level.GetComponent<SoundInfo>();

		for (auto const& [entity_id, entity_sound_info] : sound_info_map)
		{
			if (sound_buffers_.count(entity_sound_info.sound_path) == 0)
			{
				sound_buffers_[entity_sound_info.sound_path].loadFromFile(entity_sound_info.sound_path);
			}
			if (sound_info_map[entity_id].play_sound)
			{
				float max_volume = 100; //SFML uses 0-100 for volume, we use 0-1

				if (MAX_VOLUME.count(entity_sound_info.sound_path) > 0)
				{
					max_volume = MAX_VOLUME[entity_sound_info.sound_path];
					std::cout << ":::" << entity_sound_info.sound_path << "\n";
				}
				sounds_[entity_sound_info.sound_path].setBuffer(sound_buffers_[entity_sound_info.sound_path]);
				sounds_[entity_sound_info.sound_path].setVolume(max_volume * sound_info_map[entity_id].sound_volume);
				sounds_[entity_sound_info.sound_path].play();
				sound_info_map[entity_id].play_sound = false;
			}
			if (sounds_.count(entity_sound_info.sound_path) != 0)
			{
				if (sounds_[entity_sound_info.sound_path].getStatus() == sf::SoundSource::Status::Stopped)
				{
					sounds_.erase(entity_sound_info.sound_path);
				}
			}
		}
	}
	void OnEnterMode(Level& level, Mode mode) {};
	void OnExitMode(Level& level, Mode mode) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};
