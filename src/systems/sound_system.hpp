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
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, SoundInfo>& sound_info_map = level.GetComponent<SoundInfo>();

		for (auto const& [entity_id, entity_soundinfo] : sound_info_map)
		{
			if (sound_buffers_.count(entity_soundinfo.sound_path) == 0)
			{
				sound_buffers_[entity_soundinfo.sound_path].loadFromFile(entity_soundinfo.sound_path);
			}
			if (sound_info_map[entity_id].play_sound)
			{
				sounds_[entity_soundinfo.sound_path].setBuffer(sound_buffers_[entity_soundinfo.sound_path]);
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
};
