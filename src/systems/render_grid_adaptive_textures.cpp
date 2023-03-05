#include "systems/render_grid_adaptive_textures.hpp"
#include "components/physics.hpp"
#include "level.hpp"
#include <functional>
#include <optional>
#include <string>

sf::RenderTexture* RenderGridAdaptiveTexturesSystem::EnsureTextureSetup(Level& level, unsigned subsampling, std::string fragment_shader_path, int draw_priority)
{
	auto view_size = level.GetSize();
	auto texture_size = level.GetGridSize() * subsampling;

	if (textures_and_shapes_[fragment_shader_path].count(draw_priority) == 0)
	{
		std::get<int>(textures_and_shapes_[fragment_shader_path][draw_priority]) = level.CreateEntityId();
	}

	sf::RenderTexture& render_texture = std::get<sf::RenderTexture>(textures_and_shapes_[fragment_shader_path][draw_priority]);
	if (render_texture.getSize() != texture_size)
	{
		render_texture.create(texture_size.x, texture_size.y);
		render_texture.setView(sf::View(view_size / 2.f, view_size));
	};
	return &render_texture;
}

void RenderGridAdaptiveTexturesSystem::UpdateTexture(Level& level, unsigned subsampling, std::string fragment_shader_path, std::function<void(Level&, std::function<sf::RenderTexture*(int)>)> draw_func)
{
	std::function<sf::RenderTexture*(int)> get_texture = std::bind(&RenderGridAdaptiveTexturesSystem::EnsureTextureSetup, this, level, subsampling, fragment_shader_path, std::placeholders::_1);
	draw_func(level, get_texture);

	sf::RenderTexture* t = EnsureTextureSetup(level, subsampling, fragment_shader_path, 1);

	for (auto& [draw_priority, tup] : textures_and_shapes_.at(fragment_shader_path))
	{
		int entity_id = std::get<int>(tup);
		sf::RenderTexture& texture = std::get<sf::RenderTexture>(tup);
		sf::RectangleShape& shape = std::get<sf::RectangleShape>(tup);
		sf::Vector2u grid_size = texture.getSize();
		Shader* shader = level.AddComponent<Shader>(entity_id);
		shader->fragment_shader_path = fragment_shader_path;
		shader->float_uniforms["grid_width"] = grid_size.x;
		shader->float_uniforms["grid_height"] = grid_size.y;
		texture.display();
		shape.setSize(level.GetSize());
		shape.setTexture(&texture.getTexture());
		level.drawables[draw_priority].push_back({ entity_id, &shape });
	}
}

template <class T>
static void SetAndAssertSync(std::optional<T>& current, T new_val)
{
	if (current.has_value())
	{
		assert(current.value() == new_val);
	}
	else
	{
		current = new_val;
	}
}

static void LaserDrawFunc(Level& level, std::function<sf::RenderTexture*(int)> get_render_texture)
{
	for (auto const& [entity_id, laser, draw_priority, width_and_height, position] : level.GetEntitiesWith<Laser, DrawPriority, WidthAndHeight, Position>())
	{
		sf::RectangleShape shape = sf::RectangleShape(width_and_height->width_and_height);
		shape.setPosition(position->position - width_and_height->width_and_height / 2.f);
		shape.setFillColor(sf::Color(255, 0, 0, 255));
		get_render_texture(draw_priority->draw_priority)->draw(shape);
	}
}

static void WallDrawFunc(Level& level, std::function<sf::RenderTexture*(int)> get_render_texture)
{
	for (auto const& [entity_id, wall, collision, draw_priority, width_and_height, position] : level.GetEntitiesWith<Wall, Collision, DrawPriority, WidthAndHeight, Position>())
	{
		sf::RectangleShape shape = sf::RectangleShape(width_and_height->width_and_height);
		shape.setPosition(position->position - width_and_height->width_and_height / 2.f);
		shape.setFillColor(sf::Color(255, collision->bounce_factor * 255, 0, 255));
		get_render_texture(draw_priority->draw_priority)->draw(shape);
	}
}

void RenderGridAdaptiveTexturesSystem::Update(Level& level, float dt)
{
	for (auto& [fragment_shader_path, textures_and_shapes] : textures_and_shapes_)
	{
		for (auto& [draw_priority, tup] : textures_and_shapes)
		{
			sf::RenderTexture& texture = std::get<sf::RenderTexture>(tup);
			texture.clear(sf::Color(0, 0, 0, 0));
		}
	}
	UpdateTexture(level, 4u, "shaders\\adaptive_lasers.frag", LaserDrawFunc);
	UpdateTexture(level, 2u, "shaders\\adaptive_walls.frag", WallDrawFunc);
}