#pragma once
#include "constants.hpp"
#include "game_system.hpp"
#include "level.hpp"

class DrawSystem : public GameSystem
{
private:
	sf::RectangleShape background_;
	sf::Texture background_texture_;
	std::map<int, sf::Shader> shaders_;

public:
	DrawSystem()
	{
		background_texture_.loadFromFile("content\\textures\\background.png");
		background_.setTexture(&background_texture_);
	}
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& shader_map = level.GetComponent<Shader>();

		globals.render_window.clear();

		sf::Vector2f background_size = level.size * MAX_SCREEN_SIZE_SHAKE;
		background_.setSize(background_size);
		background_.setPosition(-(background_size - level.size) / 2.f);
		globals.render_window.draw(background_);

		for (const auto& [entity_id, shader] : shader_map)
		{
			if (shaders_.count(entity_id) == 0)
			{
				if (!shader.vertex_shader_path.empty() && !shader.fragment_shader_path.empty())
				{
					shaders_[entity_id].loadFromFile(shader.vertex_shader_path, shader.fragment_shader_path);
				}
				else if (!shader.vertex_shader_path.empty())
				{
					shaders_[entity_id].loadFromFile(shader.vertex_shader_path, sf::Shader::Vertex);
				}
				else if (!shader.fragment_shader_path.empty())
				{
					shaders_[entity_id].loadFromFile(shader.fragment_shader_path, sf::Shader::Fragment);
				}
				else
				{
					assert(false);
				}
			}
			for (const auto& [name, value] : shader.uniforms)
			{
				shaders_[entity_id].setUniform(name, value);
			}
		}

		for (auto [draw_priority, entity_bound_drawables] : level.drawables)
		{
			for (auto entity_bound_drawable : entity_bound_drawables)
			{
				int entity_id = entity_bound_drawable.entity_id;
				if (shader_map.count(entity_id) > 0)
				{
					globals.render_window.draw(*entity_bound_drawable.drawable, &shaders_[entity_id]);
				}
				else
				{
					globals.render_window.draw(*entity_bound_drawable.drawable);
				}
			}
		}
		level.drawables.clear();
	}
};