#include "systems/draw.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/System/Err.hpp"
#include "constants.hpp"

void DrawSystem::Update(Level& level, float dt)
{
	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Numpad0])
	{
		shaders_.clear();
	}
	Draw(level, &globals.render_window, level.drawables, true);
	globals.render_window.display();
}

void DrawSystem::CaptureLevel(Level& level, sf::Texture* texture, unsigned width, unsigned height)
{
	sf::RenderTexture render_texture;
	render_texture.create(width, height);
	render_texture.setView(sf::View(level.GetSize() / 2.f, level.GetSize()));
	Draw(level, &render_texture, level.drawables, false);
	render_texture.display();
	*texture = render_texture.getTexture();
}

void DrawSystem::Draw(Level& level, sf::RenderTarget* render_target, std::map<int, std::vector<EntityBoundDrawable>> drawables, bool use_shaders)
{
	globals.render_window.clear();

	for (auto [draw_priority, entity_bound_drawables] : drawables)
	{
		for (auto entity_bound_drawable : entity_bound_drawables)
		{
			int entity_id = entity_bound_drawable.entity_id;
			if (use_shaders && level.HasComponents<Shader>(entity_id))
			{
				render_target->draw(*entity_bound_drawable.drawable, SetupSFMLShader(level, entity_id, level.GetComponent<Shader>(entity_id)));
			}
			else
			{
				render_target->draw(*entity_bound_drawable.drawable);
			}
		}
	}
	level.drawables.clear();
}

sf::Shader* DrawSystem::SetupSFMLShader(Level& level, int entity_id, const Shader* shader)
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
	// Redirect to nothing
	std::streambuf* previous = sf::err().rdbuf(NULL);
	shaders_[shader_id].setUniform("_time", globals.time);
	shaders_[shader_id].setUniform("_window_resolution", sf::Vector2f(globals.render_window.getSize()));
	shaders_[shader_id].setUniform("_level_size", level.GetSize());
	shaders_[shader_id].setUniform("_view_size", globals.render_window.getView().getSize());
	shaders_[shader_id].setUniform("_view_center", globals.render_window.getView().getCenter());
	if (level.HasComponents<WidthAndHeight>(entity_id))
	{
		shaders_[shader_id].setUniform("_wh", level.GetComponent<WidthAndHeight>(entity_id)->width_and_height);
	}
	// Restore the original output
	sf::err().rdbuf(previous);
	return &shaders_[shader_id];
}