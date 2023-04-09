#pragma once
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include <tuple>
#include <typeindex>

class RenderGridAdaptiveTexturesSystem : public GameSystem
{
private:
	std::map<std::string, std::map<int, std::tuple<sf::RenderTexture, sf::RectangleShape, Shader>>> textures_and_shapes_;

	sf::RenderTexture& EnsureTextureSetup(Level& level, unsigned subsampling, std::string fragment_shader_path, int draw_priority);
	void UpdateTexture(Level& level, unsigned subsampling, std::string fragment_shader_path, std::function<void(Level&, std::function<sf::RenderTexture&(int)>)> draw_func);

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
};