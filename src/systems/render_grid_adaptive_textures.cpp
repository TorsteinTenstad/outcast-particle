#include "systems/render_grid_adaptive_textures.hpp"
#include "components/physics.hpp"
#include "level.hpp"
#include <functional>
#include <optional>
#include <string>

template <class TextureType>
static void UpdateTextureOfType(Level& level, sf::RectangleShape& shape, sf::RenderTexture& reder_texture, unsigned subsampling, std::string fragment_shader_path, std::function<std::optional<int>(Level&, sf::RenderTexture&)> draw_func)
{
	auto [entity_id, texture] = GetSingletonIncludeID<TextureType>(level);
	reder_texture.clear(sf::Color(0, 0, 0, 0));
	auto level_size = level.GetSize();
	auto rt_res = level.GetGridSize() * subsampling;
	if (reder_texture.getSize() != rt_res)
	{
		reder_texture.create(rt_res.x, rt_res.y);
		reder_texture.setView(sf::View(level_size / 2.f, level_size));
	};
	std::optional<int> draw_priority = draw_func(level, reder_texture);
	if (!draw_priority.has_value()) // The draw func may not find anything to draw
	{
		return;
	}
	Shader* shader = level.AddComponent<Shader>(entity_id);
	shader->fragment_shader_path = fragment_shader_path;
	shader->float_uniforms["grid_width"] = rt_res.x;
	shader->float_uniforms["grid_height"] = rt_res.y;
	reder_texture.display();
	shape = sf::RectangleShape(level_size);
	shape.setTexture(&reder_texture.getTexture());
	level.drawables[draw_priority.value()].push_back({ entity_id, &shape });
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

static std::optional<int> LaserDrawFunc(Level& level, sf::RenderTexture& render_texture)
{
	std::optional<int> synced_draw_priority;
	for (auto const& [entity_id, laser, draw_priority, width_and_height, position] : level.GetEntitiesWith<Laser, DrawPriority, WidthAndHeight, Position>())
	{
		SetAndAssertSync(synced_draw_priority, draw_priority->draw_priority);
		sf::RectangleShape shape = sf::RectangleShape(width_and_height->width_and_height);
		shape.setPosition(position->position - width_and_height->width_and_height / 2.f);
		shape.setFillColor(sf::Color(255, 0, 0, 255));

		render_texture.draw(shape);
	}
	return synced_draw_priority;
}

static std::optional<int> WallDrawFunc(Level& level, sf::RenderTexture& render_texture)
{
	std::optional<int> synced_draw_priority;
	for (auto const& [entity_id, wall, collision, draw_priority, width_and_height, position] : level.GetEntitiesWith<Wall, Collision, DrawPriority, WidthAndHeight, Position>())
	{
		SetAndAssertSync(synced_draw_priority, draw_priority->draw_priority);
		sf::RectangleShape shape = sf::RectangleShape(width_and_height->width_and_height);
		shape.setPosition(position->position - width_and_height->width_and_height / 2.f);
		shape.setFillColor(sf::Color(255, collision->bounce_factor * 255, 0, 255));
		render_texture.draw(shape);
	}
	return synced_draw_priority;
}

void RenderGridAdaptiveTexturesSystem::Update(Level& level, float dt)
{
	UpdateTextureOfType<LaserTexture>(level, laser_shape_, laser_render_texture_, 4u, "shaders\\adaptive_lasers.frag", LaserDrawFunc);
	UpdateTextureOfType<WallTexture>(level, wall_shape_, wall_render_texture_, 2u, "shaders\\adaptive_walls.frag", WallDrawFunc);
}