#pragma once
#include "SFML/Graphics/Shader.hpp"
#include "constants.hpp"
#include "game_system.hpp"
#include "level.hpp"

class DrawSystem : public GameSystem
{
private:
	std::map<int, sf::Shader> shaders_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		auto& shader_map = level.GetComponent<Shader>();

		globals.render_window.clear();

		for (auto& [entity_id, shader] : shader_map)
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
			for (const auto& [name, value] : shader.float_uniforms)
			{
				if (name == "_time")
				{
					shaders_[entity_id].setUniform("_time", globals.time);
				}
				else
				{
					shaders_[entity_id].setUniform(name, value);
				}
			}
			for (const auto& [name, value] : shader.int_uniforms)
			{
				shaders_[entity_id].setUniform(name, value);
			}
			for (const auto& [name, value] : shader.vec_uniforms)
			{
				shaders_[entity_id].setUniform(name, value);
			}
		}

		for (auto [draw_priority, entity_bound_drawables] : level.drawables)
		{
			for (auto entity_bound_drawable : entity_bound_drawables)
			{
				int entity_id = entity_bound_drawable.entity_id;
				if (shader_map.count(entity_id) > 0) // && mode_ == PLAY_MODE)
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
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};