#pragma once
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class RenderTextSystem : public GameSystem
{
private:
	std::map<Entity, sf::Text> text_;
	std::map<std::string, std::map<unsigned int, float>> text_height_;
	std::map<std::string, sf::Font> font_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
};