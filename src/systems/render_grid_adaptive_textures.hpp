#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "game_system.hpp"
#include "level.hpp"

class RenderGridAdaptiveTexturesSystem : public GameSystem
{
private:
	sf::RenderTexture wall_render_texture_;
	sf::RectangleShape wall_shape_;
	sf::RenderTexture laser_render_texture_;
	sf::RectangleShape laser_shape_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
};