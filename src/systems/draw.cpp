#include "systems/draw.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/System/Err.hpp"
#include "components/draw_info.hpp"
#include "components/shader.hpp"
#include "components/size.hpp"
#include "constants.hpp"
#include <cassert>

void DrawSystem::Update(Level& level, float dt)
{
	if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.RESET_SHADERS_AND_TEXTURES])
	{
		shader_manager_.Clear();
	}
	Draw(level, &globals.render_window, level.drawables, true);
	globals.render_window.display();
}

void DrawSystem::CaptureLevel(Level& level, sf::Texture* texture, unsigned width, unsigned height)
{
	sf::RenderTexture render_texture;
	render_texture.create(width, height);
	render_texture.setView(sf::View(level.GetSize() / 2.f, level.GetSize()));
	Draw(level, &render_texture, level.drawables, true);
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
			if (use_shaders
				&& entity_bound_drawable.entity_id.has_value()
				&& level.HasComponents<Shader>(entity_bound_drawable.entity_id.value()))
			{
				render_target->draw(*entity_bound_drawable.drawable, SetupSFMLShader(level, level.GetComponent<Shader>(entity_bound_drawable.entity_id.value()), entity_bound_drawable.entity_id));
			}
			else if (use_shaders
					 && entity_bound_drawable.shader.has_value())
			{
				render_target->draw(*entity_bound_drawable.drawable, SetupSFMLShader(level, entity_bound_drawable.shader.value(), entity_bound_drawable.entity_id));
			}
			else
			{
				render_target->draw(*entity_bound_drawable.drawable);
			}
		}
	}
	level.drawables.clear();
}

sf::Shader* DrawSystem::SetupSFMLShader(Level& level, const Shader* shader, std::optional<int> entity_id)
{
	sf::Shader& sfml_shader = shader_manager_.GetLoadedSFMLShader(shader->vertex_shader_path, shader->fragment_shader_path);
	for (const auto& [name, value] : shader->float_uniforms)
	{
		sfml_shader.setUniform(name, value);
	}
	for (const auto& [name, value] : shader->int_uniforms)
	{
		sfml_shader.setUniform(name, value);
	}
	for (const auto& [name, value] : shader->vec_uniforms)
	{
		sfml_shader.setUniform(name, value);
	}
	for (const auto& [name, value] : shader->vec4_uniforms)
	{
		sfml_shader.setUniform(name, value);
	}
	// Redirect to nothing
	std::streambuf* previous = sf::err().rdbuf(NULL);
	sfml_shader.setUniform("_time", globals.time);
	sfml_shader.setUniform("_window_resolution", sf::Vector2f(globals.render_window.getSize()));
	sfml_shader.setUniform("_level_size", level.GetSize());
	sfml_shader.setUniform("_view_size", globals.render_window.getView().getSize());
	sfml_shader.setUniform("_view_center", globals.render_window.getView().getCenter());
	sfml_shader.setUniform("_texture", sf::Shader::CurrentTexture);
	sfml_shader.setUniform("_noise_texture", noise_texture_);
	if (WidthAndHeight* width_and_height = level.RawGetComponent<WidthAndHeight>(entity_id.value_or(-1)))
	{
		sfml_shader.setUniform("_wh", width_and_height->width_and_height);
	}
	if (Radius* radius = level.RawGetComponent<Radius>(entity_id.value_or(-1)))
	{
		sfml_shader.setUniform("_wh", sf::Vector2f(2, 2) * radius->radius);
	}
	// Restore the original output
	sf::err().rdbuf(previous);
	return &sfml_shader;
}