#pragma once
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
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
		Draw(level, &globals.render_window, level.drawables, true);
		globals.render_window.display();
	}

	void CaptureLevel(Level& level, sf::Texture* texture, unsigned width, unsigned height)
	{
		sf::RenderTexture render_texture;
		render_texture.create(width, height);
		render_texture.setView(sf::View(level.GetSize() / 2.f, level.GetSize()));
		Draw(level, &render_texture, level.drawables, false);
		render_texture.display();
		*texture = render_texture.getTexture();
	}

	void Draw(Level& level, sf::RenderTarget* render_target, std::map<int, std::vector<EntityBoundDrawable>> drawables, bool use_shaders)
	{
		auto& shader_map = level.GetComponent<Shader>();

		globals.render_window.clear();

		for (auto [draw_priority, entity_bound_drawables] : drawables)
		{
			for (auto entity_bound_drawable : entity_bound_drawables)
			{
				int entity_id = entity_bound_drawable.entity_id;
				if (use_shaders && shader_map.count(entity_id) > 0)
				{
					render_target->draw(*entity_bound_drawable.drawable, SetupSFMLShader(level, entity_id, &shader_map[entity_id]));
				}
				else
				{
					render_target->draw(*entity_bound_drawable.drawable);
				}
			}
		}
		level.drawables.clear();
	}

private:
	sf::Shader* SetupSFMLShader(Level& level, int entity_id, const Shader* shader)
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
			shaders_[shader_id].setUniform(name, value);
		}
		for (const auto& [name, value] : shader->int_uniforms)
		{
			shaders_[shader_id].setUniform(name, value);
		}
		for (const auto& [name, value] : shader->vec_uniforms)
		{
			shaders_[shader_id].setUniform(name, value);
		}
		shaders_[shader_id].setUniform("_time", globals.time);
		if (level.HasComponents<WidthAndHeight>(entity_id))
		{
			shaders_[shader_id].setUniform("_wh", level.GetComponent<WidthAndHeight>()[entity_id].width_and_height);
		}
		return &shaders_[shader_id];
	}
};