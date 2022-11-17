#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

class SFMLRenderSystem : public GameSystem
{
private:
	std::map<std::string, sf::Texture> textures_;
	std::map<int, sf::RectangleShape> rectangle_shapes_;
	std::map<int, sf::CircleShape> circle_shapes_;
	std::map<int, std::vector<int>> draw_order_;

public:
	void RegisterTexture(std::string identifier, sf::Texture texture)
	{
		textures_[identifier] = texture;
	}
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)cursor_and_keys;
		(void)dt;
		std::map<int, DrawInfo>& draw_info_map = level.GetComponent<DrawInfo>();
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, WidthAndHeight>& width_and_height_map = level.GetComponent<WidthAndHeight>();
		std::map<int, Radius>& radius_map = level.GetComponent<Radius>();
		std::map<int, Border>& border_map = level.GetComponent<Border>();

		draw_order_.clear();
		globals.render_window.setView(sf::View(level.size / 2.f, level.size));
		for (auto const& [entity_id, entity_drawinfo] : draw_info_map)
		{
			draw_order_[entity_drawinfo.draw_priority].push_back(entity_id);
			if (textures_.count(entity_drawinfo.image_path) == 0)
			{
				textures_[entity_drawinfo.image_path] = sf::Texture();
				textures_[entity_drawinfo.image_path].loadFromFile(entity_drawinfo.image_path);
				textures_[entity_drawinfo.image_path].setRepeated(true);
				textures_[entity_drawinfo.image_path].setSmooth(true);
			}
			if (width_and_height_map.count(entity_id))
			{
				if (rectangle_shapes_.count(entity_id) == 0)
				{
					rectangle_shapes_[entity_id] = sf::RectangleShape();
				}
				rectangle_shapes_[entity_id].setSize(width_and_height_map[entity_id].width_and_height);
				if (!entity_drawinfo.scale_to_fit)
				{
					rectangle_shapes_[entity_id].setTextureRect(sf::IntRect(0, 0, width_and_height_map[entity_id].width_and_height.x, width_and_height_map[entity_id].width_and_height.y));
				}
				rectangle_shapes_[entity_id].setTexture(&textures_[entity_drawinfo.image_path]);
				rectangle_shapes_[entity_id].setOrigin(width_and_height_map[entity_id].width_and_height.x / 2, width_and_height_map[entity_id].width_and_height.y / 2);
				rectangle_shapes_[entity_id].setPosition(position_map[entity_id].position);
				if (border_map.count(entity_id))
				{
					rectangle_shapes_[entity_id].setOutlineThickness(border_map[entity_id].thickness);
					rectangle_shapes_[entity_id].setOutlineColor(border_map[entity_id].color);
				}
				else
				{
					rectangle_shapes_[entity_id].setOutlineThickness(0);
				}
			}
			else if (radius_map.count(entity_id))
			{
				if (circle_shapes_.count(entity_id) == 0)
				{
					circle_shapes_[entity_id] = sf::CircleShape();
				}
				circle_shapes_[entity_id].setRadius(radius_map[entity_id].radius);
				if (!entity_drawinfo.scale_to_fit)
				{
					circle_shapes_[entity_id].setTextureRect(sf::IntRect(0, 0, 2 * radius_map[entity_id].radius, 2 * radius_map[entity_id].radius));
				}
				circle_shapes_[entity_id].setTexture(&textures_[entity_drawinfo.image_path]);
				circle_shapes_[entity_id].setOrigin(radius_map[entity_id].radius, radius_map[entity_id].radius);
				circle_shapes_[entity_id].setPosition(position_map[entity_id].position);
				if (border_map.count(entity_id))
				{
					circle_shapes_[entity_id].setOutlineThickness(border_map[entity_id].thickness);
					circle_shapes_[entity_id].setOutlineColor(border_map[entity_id].color);
				}
				else
				{
					circle_shapes_[entity_id].setOutlineThickness(0);
				}
			}
		}
		for (auto [draw_priority, entity_ids] : draw_order_)
		{
			for (auto entity_id : entity_ids)
			{
				if (rectangle_shapes_.count(entity_id) != 0)
				{
					globals.render_window.draw(rectangle_shapes_[entity_id]);
				}
				if (circle_shapes_.count(entity_id) != 0)
				{
					globals.render_window.draw(circle_shapes_[entity_id]);
				}
			}
		}
	}
};