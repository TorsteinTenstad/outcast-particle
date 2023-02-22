#pragma once
#include "systems/render_grid_adaptive_textures.hpp"
#include "components/physics.hpp"
#include "level.hpp"

void RenderGridAdaptiveTexturesSystem::Update(Level& level, float dt)
{
	auto [entity_id, wall_texture] = GetSingletonIncludeID<WallTexture>(level);
	wall_render_texture_.clear(sf::Color(0, 0, 0, 0));
	auto level_size = level.GetSize();
	auto rt_res = level.GetGridSize() * 2u;
	if (wall_render_texture_.getSize() != rt_res)
	{
		wall_render_texture_.create(rt_res.x, rt_res.y);
		wall_render_texture_.setView(sf::View(level_size / 2.f, level_size));
	};
	Shader* shader = level.AddComponent<Shader>(entity_id);
	shader->fragment_shader_path = "shaders\\adaptive_walls.frag";
	shader->float_uniforms["grid_width"] = rt_res.x;
	shader->float_uniforms["grid_height"] = rt_res.y;
	shader->float_uniforms["grid_size"] = float(BLOCK_SIZE) / 2.f;
	for (auto const& [entity_id, collision, width_and_height, position] : level.GetEntitiesWith<Collision, WidthAndHeight, Position>())
	{
		sf::RectangleShape shape = sf::RectangleShape(width_and_height->width_and_height);
		shape.setPosition(position->position - width_and_height->width_and_height / 2.f);
		shape.setFillColor(sf::Color(255, collision->bounce_factor * 255, 0, 255));
		wall_render_texture_.draw(shape);
	}
	wall_render_texture_.display();
	wall_shape_ = sf::RectangleShape(level_size);
	wall_shape_.setTexture(&wall_render_texture_.getTexture());
	level.drawables[4].push_back({ entity_id, &wall_shape_ });
}