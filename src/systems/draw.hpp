#pragma once
#include "SFML/Graphics/Shader.hpp"
#include "game_system.hpp"
#include "level.hpp"

class DrawSystem : public GameSystem
{
private:
	std::map<std::tuple<std::string, std::string>, sf::Shader> shaders_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
	void CaptureLevel(Level& level, sf::Texture* texture, unsigned width, unsigned height);

private:
	void Draw(Level& level, sf::RenderTarget* render_target, std::map<int, std::vector<EntityBoundDrawable>> drawables, bool use_shaders);
	sf::Shader* SetupSFMLShader(Level& level, int entity_id, const Shader* shader);
};