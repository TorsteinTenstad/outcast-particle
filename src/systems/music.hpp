#pragma once
#include "SFML/Audio/Music.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class MusicSystem : public GameSystem
{
private:
	std::string music_path_;
	sf::Music music;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
};
