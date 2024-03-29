#pragma once
#include "systems/render_shapes.hpp"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "components/size.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "render_shapes.hpp"
#include "utils/get_size.hpp"
#include <cassert>

sf::Texture* RenderShapesSystem::GetLoadedSFMLTexture(std::string image_path)
{
	if (textures_.count(image_path) == 0)
	{
		bool loaded = textures_[image_path].loadFromFile(image_path);
		if (!loaded)
		{
			textures_[image_path].loadFromFile((TEXTURES_DIR / "white.png").string());
		}
		textures_[image_path].setSmooth(true);
	}
	return &textures_.at(image_path);
}

sf::Texture* RenderShapesSystem::RegisterTexture(std::string identifier)
{
	return &textures_[identifier];
}

EntityBoundDrawable RenderShapesSystem::RenderShape(Entity entity, sf::Shape* shape, sf::Texture* texture, sf::Vector2f w_h, sf::Color fill_color, bool tile, Position* position)
{
	if (tile)
	{
		shape->setTextureRect(sf::IntRect(0, 0, w_h.x, w_h.y));
	}
	else
	{
		sf::Vector2u texture_size = texture->getSize();
		shape->setTextureRect(sf::IntRect(0, 0, texture_size.x, texture_size.y));
	}
	texture->setRepeated(tile);
	shape->setOrigin(w_h.x / 2, w_h.y / 2);
	shape->setTexture(texture);
	shape->setPosition(position->position);
	shape->setFillColor(fill_color);
	return { shape, entity };
}

void RenderShapesSystem::Update(Level& level, float dt)
{
	if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.RESET_SHADERS_AND_TEXTURES])
	{
		textures_.clear();
	}
	for (auto const& [entity, draw_info, draw_priority, position] : level.GetEntitiesWith<DrawInfo, DrawPriority, Position>())
	{
		sf::Shape* shape;
		if (WidthAndHeight* width_and_height = level.RawGetComponent<WidthAndHeight>(entity))
		{
			rectangle_shapes_[entity].setSize(width_and_height->width_and_height);
			shape = &rectangle_shapes_[entity];
		}
		else if (Radius* radius = level.RawGetComponent<Radius>(entity))
		{
			circle_shapes_[entity].setRadius(radius->radius);
			shape = &circle_shapes_[entity];
		}
		else
		{
			assert(false);
		}

		sf::Texture* texture = GetLoadedSFMLTexture(draw_info->image_path.value_or(""));
		sf::Vector2f size = GetSize(level, entity, false);
		sf::Color fill_color = level.HasComponents<FillColor>(entity) ? level.GetComponent<FillColor>(entity)->color : sf::Color::White;
		EntityBoundDrawable entity_bound_drawable = RenderShape(entity, shape, texture, size, fill_color, draw_info->tile, position);

		level.drawables[draw_priority->draw_priority].push_back(entity_bound_drawable);
	}
}