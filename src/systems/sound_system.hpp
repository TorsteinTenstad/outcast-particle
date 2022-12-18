#pragma once
#include "Components/sound_info.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class SoundSystem : public GameSystem
{
private:
	std::map<std::string, sf::SoundBuffer> sound_buffers_;
	std::map<std::string, sf::Sound> sounds_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& sound_info_map = level.GetComponent<SoundInfo>();

		for (auto const& [entity_id, entity_soundinfo] : sound_info_map)
		{
			if (sound_buffers_.count(entity_soundinfo.sound_path) == 0)
			{
				sound_buffers_[entity_soundinfo.sound_path].loadFromFile(entity_soundinfo.sound_path);
			}
			if (sound_info_map[entity_id].play_sound)
			{
				sounds_[entity_soundinfo.sound_path].setBuffer(sound_buffers_[entity_soundinfo.sound_path]);
				sounds_[entity_soundinfo.sound_path].setVolume(100 * sound_info_map[entity_id].sound_volume); //SFML uses 0-100 for volume, we use 0-1
				sounds_[entity_soundinfo.sound_path].play();
				sound_info_map[entity_id].play_sound = false;
			}
			if (sounds_.count(entity_soundinfo.sound_path) != 0)
			{
				if (sounds_[entity_soundinfo.sound_path].getStatus() == sf::SoundSource::Status::Stopped)
				{
					sounds_.erase(entity_soundinfo.sound_path);
				}
			}
		}
	}
	void OnEnterMode(Mode mode) {};
	void OnExitMode(Mode mode) {};
	void OnEnterLevel(int level_id) {};
	void OnExitLevel(int level_id) {};
};
