#include "systems/render_grid_adaptive_textures.hpp"
#include "components/animated_properties.hpp"
#include "components/collision.hpp"
#include "components/draw_info.hpp"
#include "components/grid_adaptive_textures.hpp"
#include "components/laser.hpp"
#include "components/physics.hpp"
#include "components/size.hpp"
#include "level.hpp"
#include "utils/math.hpp"
#include <functional>
#include <optional>
#include <string>

sf::RenderTexture& RenderGridAdaptiveTexturesSystem::EnsureTextureSetup(Level& level, unsigned subsampling, std::string fragment_shader_path, int draw_priority)
{
	auto view_size = level.GetSize();
	auto texture_size = level.GetGridSize() * subsampling;

	sf::RenderTexture& render_texture = std::get<sf::RenderTexture>(textures_and_shapes_[fragment_shader_path][draw_priority]);
	if (render_texture.getSize() != texture_size)
	{
		render_texture.create(texture_size.x, texture_size.y);
		render_texture.setView(sf::View(view_size / 2.f, view_size));
	};
	return render_texture;
}

void RenderGridAdaptiveTexturesSystem::UpdateTexture(Level& level, unsigned subsampling, std::string fragment_shader_path, std::function<void(Level&, std::function<sf::RenderTexture&(int)>)> draw_func)
{
	std::function<sf::RenderTexture&(int)> get_texture = std::bind(&RenderGridAdaptiveTexturesSystem::EnsureTextureSetup, this, std::ref(level), subsampling, fragment_shader_path, std::placeholders::_1);
	draw_func(level, get_texture);

	if (textures_and_shapes_.count(fragment_shader_path) == 0)
	{ // The draw function may not draw anything
		return;
	}

	for (auto& [draw_priority, tup] : textures_and_shapes_.at(fragment_shader_path))
	{
		Shader& shader = std::get<Shader>(tup);
		sf::RenderTexture& texture = std::get<sf::RenderTexture>(tup);
		sf::RectangleShape& shape = std::get<sf::RectangleShape>(tup);
		sf::Vector2u grid_size = texture.getSize();
		shader.fragment_shader_path = fragment_shader_path;
		shader.float_uniforms["grid_width"] = grid_size.x;
		shader.float_uniforms["grid_height"] = grid_size.y;
		texture.display();
		shape.setSize(level.GetSize());
		shape.setTexture(&texture.getTexture());
		shape.setTextureRect(sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
		level.drawables[draw_priority].push_back({ &shape, std::optional<Entity>(), &shader });
	}
}

static void LaserDrawFunc(Level& level, std::function<sf::RenderTexture&(int)> get_render_texture)
{
	for (auto const& [entity, laser, draw_priority, width_and_height, position] : level.GetEntitiesWith<Laser, DrawPriority, WidthAndHeight, Position>())
	{
		sf::RectangleShape shape = sf::RectangleShape(width_and_height->width_and_height);
		shape.setPosition(position->position - width_and_height->width_and_height / 2.f);
		float alpha = 255;
		if (FillColor* fill_color = level.RawGetComponent<FillColor>(entity))
		{
			alpha = fill_color->color.a;
		}
		shape.setFillColor(sf::Color(255, 0, alpha, 255)); //Since drawing on the image does alpha blending, using the alpha channel to encode transparency affects the other channels.
		get_render_texture(draw_priority->draw_priority).draw(shape);
	}
}

static void WallDrawFunc(Level& level, std::function<sf::RenderTexture&(int)> get_render_texture)
{
	for (auto const& [entity, wall, collision, draw_priority, width_and_height, position] : level.GetEntitiesWith<Wall, Collision, DrawPriority, WidthAndHeight, Position>())
	{
		sf::RectangleShape shape = sf::RectangleShape(width_and_height->width_and_height);
		shape.setPosition(position->position - width_and_height->width_and_height / 2.f);
		shape.setFillColor(sf::Color(255, std::min(1.f, collision->bounce_factor / 2) * 255, 0, 255));
		get_render_texture(draw_priority->draw_priority).draw(shape);
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