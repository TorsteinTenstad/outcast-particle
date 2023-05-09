#pragma once
#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"
#include "SFML/Audio/SoundSource.hpp"
#include "components/sound_info.hpp"
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
	void Update(Level& level, float dt);
};
