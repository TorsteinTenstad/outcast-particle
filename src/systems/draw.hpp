#pragma once
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "cursor_and_keys.hpp"
#include "folder_definitions.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "shader_manager.hpp"
#include <optional>
#include <string>

class DrawSystem : public GameSystem
{
private:
	ShaderManager shader_manager_;
	sf::Texture noise_texture_;

public:
	DrawSystem(std::string& active_level_id, bool& is_in_level_editing, CursorAndKeys& cursor_and_keys) :
		GameSystem(active_level_id, is_in_level_editing, cursor_and_keys)
	{
		noise_texture_.loadFromFile((TEXTURES_DIR / "noise.png").string());
	}
	void Update(Level& level, float dt);
	void CaptureLevel(Level& level, sf::Texture* texture, unsigned width, unsigned height);

private:
	void Draw(Level& level, sf::RenderTarget* render_target, std::map<int, std::vector<EntityBoundDrawable>> drawables, bool use_shaders);
	sf::Shader* SetupSFMLShader(Level& level, const Shader* shader, std::optional<Entity> entity);
};