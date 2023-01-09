#pragma once
#include "SFML/Graphics/Shader.hpp"
#include "constants.hpp"
#include "game_system.hpp"
#include "level.hpp"

class DrawSystem : public GameSystem
{
private:
	std::map<std::tuple<int, std::string, std::string>, sf::Shader> shaders_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& shader_map = level.GetComponent<Shader>();

		globals.render_window.clear();

		for (auto [draw_priority, entity_bound_drawables] : level.drawables)
		{
			for (auto entity_bound_drawable : entity_bound_drawables)
			{
				int entity_id = entity_bound_drawable.entity_id;
				if (shader_map.count(entity_id) > 0) // && mode_ == PLAY_MODE)
				{
					globals.render_window.draw(*entity_bound_drawable.drawable, SetupSFMLShader(entity_id, &shader_map[entity_id]));
				}
				else
				{
					globals.render_window.draw(*entity_bound_drawable.drawable);
				}
			}
		}
		level.drawables.clear();
	}

	sf::Shader* SetupSFMLShader(int entity_id, const Shader* shader)
	{
		std::tuple<int, std::string, std::string> shader_id = { entity_id, shader->vertex_shader_path, shader->fragment_shader_path };
		if (shaders_.count(shader_id) == 0)
		{
			if (!shader->vertex_shader_path.empty() && !shader->fragment_shader_path.empty())
			{
				shaders_[shader_id].loadFromFile(shader->vertex_shader_path, shader->fragment_shader_path);
			}
			else if (!shader->vertex_shader_path.empty())
			{
				shaders_[shader_id].loadFromFile(shader->vertex_shader_path, sf::Shader::Vertex);
			}
			else if (!shader->fragment_shader_path.empty())
			{
				shaders_[shader_id].loadFromFile(shader->fragment_shader_path, sf::Shader::Fragment);
			}
			else
			{
				assert(false);
			}
		}
		for (const auto& [name, value] : shader->float_uniforms)
		{
			if (name == "_time")
			{
				shaders_[shader_id].setUniform("_time", globals.time);
			}
			else
			{
				shaders_[shader_id].setUniform(name, value);
			}
		}
		for (const auto& [name, value] : shader->int_uniforms)
		{
			shaders_[shader_id].setUniform(name, value);
		}
		for (const auto& [name, value] : shader->vec_uniforms)
		{
			shaders_[shader_id].setUniform(name, value);
		}
		return &shaders_[shader_id];
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};