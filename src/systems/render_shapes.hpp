#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class RenderShapesSystem : public GameSystem
{
private:
	std::map<std::string, sf::Texture> textures_;
	std::map<int, sf::RectangleShape> rectangle_shapes_;
	std::map<int, sf::CircleShape> circle_shapes_;

public:
	using GameSystem::GameSystem;
	void RegisterTexture(std::string identifier, sf::Texture texture)
	{
		textures_[identifier] = texture;
	}
	void Update(Level& level, float dt)
	{
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& position_map = level.GetComponent<Position>();
		auto& width_and_height_map = level.GetComponent<WidthAndHeight>();
		auto& radius_map = level.GetComponent<Radius>();
		auto& border_map = level.GetComponent<Border>();

		for (auto const& [entity_id, entity_drawinfo] : draw_info_map)
		{
			assert(draw_priority_map.count(entity_id) > 0);
			sf::Shape* shape;
			if (textures_.count(entity_drawinfo.image_path) == 0)
			{
				textures_[entity_drawinfo.image_path] = sf::Texture();
				textures_[entity_drawinfo.image_path].loadFromFile(entity_drawinfo.image_path);
				textures_[entity_drawinfo.image_path].setSmooth(true);
			}
			if (width_and_height_map.count(entity_id))
			{
				textures_[entity_drawinfo.image_path].setRepeated(true);
				sf::Vector2f w_h = width_and_height_map[entity_id].width_and_height;
				if (entity_drawinfo.quarter_turn_rotations % 2 == 1)
				{
					w_h.x = width_and_height_map[entity_id].width_and_height.y;
					w_h.y = width_and_height_map[entity_id].width_and_height.x;
				}
				rectangle_shapes_[entity_id].setSize(w_h);
				if (entity_drawinfo.scale_to_fit)
				{
					sf::Vector2u texture_size = textures_[entity_drawinfo.image_path].getSize();
					rectangle_shapes_[entity_id].setTextureRect(sf::IntRect(0, 0, texture_size.x, texture_size.y));
				}
				else
				{
					rectangle_shapes_[entity_id].setTextureRect(sf::IntRect(0, 0, w_h.x, w_h.y));
				}
				rectangle_shapes_[entity_id].setOrigin(w_h.x / 2, w_h.y / 2);
				shape = &rectangle_shapes_[entity_id];
			}
			else if (radius_map.count(entity_id))
			{
				circle_shapes_[entity_id].setRadius(radius_map[entity_id].radius);
				if (!entity_drawinfo.scale_to_fit)

				{
					circle_shapes_[entity_id].setTextureRect(sf::IntRect(0, 0, 2 * radius_map[entity_id].radius, 2 * radius_map[entity_id].radius));
				}
				circle_shapes_[entity_id].setOrigin(radius_map[entity_id].radius, radius_map[entity_id].radius);
				shape = &circle_shapes_[entity_id];
			}
			shape->setTexture(&textures_[entity_drawinfo.image_path]);
			shape->setPosition(position_map[entity_id].position);
			shape->setRotation(90 * entity_drawinfo.quarter_turn_rotations);
			if (border_map.count(entity_id))
			{
				shape->setOutlineThickness(border_map[entity_id].thickness);
				shape->setOutlineColor(border_map[entity_id].color);
			}
			else
			{
				shape->setOutlineThickness(0);
			}
			level.drawables[draw_priority_map[entity_id].draw_priority].push_back({ entity_id, shape });
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};