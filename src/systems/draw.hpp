#pragma once
#include "SFML/Graphics/Shader.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include <optional>
#include <string>

class DrawSystem : public GameSystem
{
private:
	std::map<std::tuple<std::string, std::string>, sf::Shader> shaders_;
	sf::Texture noise_texture_;

public:
	DrawSystem(std::string& active_level_id, bool& is_in_level_editing, CursorAndKeys& cursor_and_keys) :
		GameSystem(active_level_id, is_in_level_editing, cursor_and_keys)
	{
		noise_texture_.loadFromFile("content\\textures\\noise.png");
	}
	void Update(Level& level, float dt);
	void CaptureLevel(Level& level, sf::Texture* texture, unsigned width, unsigned height);

private:
	void Draw(Level& level, sf::RenderTarget* render_target, std::map<int, std::vector<EntityBoundDrawable>> drawables, bool use_shaders);
	sf::Shader* SetupSFMLShader(Level& level, const Shader* shader, std::optional<int> entity_id);
};