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
	sf::Font font_;
	std::map<std::string, sf::Texture> textures_;
	std::map<int, sf::RectangleShape> rectangle_shapes_;
	std::map<int, sf::CircleShape> circle_shapes_;
	std::map<int, sf::Text> text_;
	std::map<int, std::vector<int>> draw_order_;

	sf::RectangleShape background_;
	sf::Texture background_texture_;
	sf::RectangleShape grid_background_;
	sf::Texture grid_background_texture_;

public:
	SFMLRenderSystem()
	{
		background_texture_.loadFromFile("content\\textures\\background.png");
		background_texture_.setRepeated(true);
		background_.setTexture(&background_texture_);
		grid_background_texture_.loadFromFile("content\\textures\\grid.png");
		grid_background_texture_.setRepeated(true);
		grid_background_.setTexture(&grid_background_texture_);
	}
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
		std::map<int, Trail>& trail_map = level.GetComponent<Trail>();
		std::map<int, Text>& text_map = level.GetComponent<Text>();

		draw_order_.clear();
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
			sf::Shape* shape;
			if (width_and_height_map.count(entity_id))
			{
				sf::Vector2f w_h = width_and_height_map[entity_id].width_and_height;
				if (entity_drawinfo.quarter_turn_rotations % 2 == 1)
				{
					w_h.x = width_and_height_map[entity_id].width_and_height.y;
					w_h.y = width_and_height_map[entity_id].width_and_height.x;
				}
				rectangle_shapes_[entity_id].setSize(w_h);
				if (!entity_drawinfo.scale_to_fit)
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
		}
		for (auto const& [entity_id, entity_text] : text_map)
		{
			draw_order_[1000].push_back(entity_id);
			font_.loadFromFile("content\\Roboto-Medium.ttf");
			text_[entity_id].setString(entity_text.content);
			text_[entity_id].setFont(font_);
			text_[entity_id].setCharacterSize(entity_text.size);
			sf::FloatRect bounds = text_[entity_id].getLocalBounds();
			text_[entity_id].setOrigin(bounds.width / 2 + bounds.left, bounds.height / 2 + bounds.top);
			text_[entity_id].setPosition(position_map[entity_id].position);
		}
		sf::Vector2f background_size = level.size * MAX_SCREEN_SIZE_SHAKE;
		background_.setSize(background_size);
		background_.setPosition(-(background_size - level.size) / 2.f);
		globals.render_window.draw(background_);
		if (level.edit_mode)
		{
			grid_background_.setSize(background_size);
			grid_background_.setTextureRect(sf::IntRect(0, 0, background_size.x, background_size.y));
			globals.render_window.draw(grid_background_);
		}
		for (auto [draw_priority, entity_ids] : draw_order_)
		{
			for (auto entity_id : entity_ids)
			{
				if (trail_map.count(entity_id) > 0)
				{
					sf::ConvexShape segment;
					sf::Vector2f global_segment_position = position_map[entity_id].position;
					sf::Vector2f p1 = global_segment_position;
					sf::Vector2f p2 = global_segment_position;
					sf::Vector2f p3;
					sf::Vector2f p4;
					sf::Vector2f unit_normal = GetQuarterTurnRotation(Normalized(trail_map[entity_id].path[0]));
					p1 -= trail_map[entity_id].widths[0] * unit_normal;
					p2 += trail_map[entity_id].widths[0] * unit_normal;
					for (unsigned i = 0; i < (trail_map[entity_id].path.size() - 1); ++i)
					{
						unit_normal = GetQuarterTurnRotation(Normalized(trail_map[entity_id].path[i + 1]));
						p3 = global_segment_position + trail_map[entity_id].path[i] + trail_map[entity_id].widths[i + 1] * unit_normal;
						p4 = global_segment_position + trail_map[entity_id].path[i] - trail_map[entity_id].widths[i + 1] * unit_normal;
						segment = sf::ConvexShape();
						segment.setPointCount(4);
						segment.setPoint(0, p1);
						segment.setPoint(1, p2);
						segment.setPoint(2, p3);
						segment.setPoint(3, p4);
						segment.setFillColor(sf::Color(200, 200, 200, 127 * pow((float)(trail_map[entity_id].path.size() - i) / trail_map[entity_id].path.size(), 2)));
						globals.render_window.draw(segment);
						global_segment_position += trail_map[entity_id].path[i];
						p1 = p4;
						p2 = p3;
					}
				}
				if (rectangle_shapes_.count(entity_id) > 0)
				{
					globals.render_window.draw(rectangle_shapes_[entity_id]);
				}
				if (circle_shapes_.count(entity_id) > 0)
				{
					globals.render_window.draw(circle_shapes_[entity_id]);
				}
				if (text_.count(entity_id) > 0)
				{
					globals.render_window.draw(text_[entity_id]);
				}
			}
		}
	}
};