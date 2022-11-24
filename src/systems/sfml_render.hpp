#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"

#define FORCE_VISUALIZATION_RESOLUTION 64

class SFMLRenderSystem : public GameSystem
{
private:
	std::map<std::string, sf::Texture> textures_;
	std::map<int, sf::RectangleShape> rectangle_shapes_;
	std::map<int, sf::CircleShape> circle_shapes_;
	std::map<int, std::vector<int>> draw_order_;

	sf::RectangleShape background_;
	sf::Texture background_texture_;
	sf::RectangleShape grid_background_;
	sf::Texture grid_background_texture_;

	float i = 0;

public:
	SFMLRenderSystem()
	{
		background_texture_.loadFromFile("content/background.png");
		background_texture_.setRepeated(true);
		background_.setTexture(&background_texture_);
		grid_background_texture_.loadFromFile("content/grid.png");
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
		std::map<int, VisualizedForces>& visualized_forces_map = level.GetComponent<VisualizedForces>();

		draw_order_.clear();
		globals.render_window.setView(sf::View(level.size / 2.f, level.size));
		for (const auto& [entity_id, entity_drawinfo] : draw_info_map)
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
		background_.setSize(level.size);
		globals.render_window.draw(background_);
		if (level.edit_mode)
		{
			grid_background_.setSize(level.size);
			grid_background_.setTextureRect(sf::IntRect(0, 0, level.size.x, level.size.y));
			globals.render_window.draw(grid_background_);
		}
		for (const auto& [draw_priority, entity_ids] : draw_order_)
		{
			for (const auto& entity_id : entity_ids)
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

		for (const auto& [entity_id, visualized_forces] : visualized_forces_map)
		{
			std::array<float, FORCE_VISUALIZATION_RESOLUTION> visualization_magnitude = {};
			for (const auto& force : visualized_forces.forces)
			{
				float force_magnitude = Magnitude(force);
				float force_angle = -Angle(force) + PI;
				for (int angle_idx = 0; angle_idx < FORCE_VISUALIZATION_RESOLUTION; ++angle_idx)
				{
					float angle = (((float)angle_idx) / FORCE_VISUALIZATION_RESOLUTION) * 2 * PI;
					float angular_diff = abs(angle - force_angle);
					if (angular_diff > PI)
					{
						angular_diff = 2 * PI - angular_diff;
					}
					if (angular_diff < 0.4)
					{
						visualization_magnitude[angle_idx] += force_magnitude;
					}
				}
			}
			std::array<sf::Vector2f, FORCE_VISUALIZATION_RESOLUTION> outer_points;
			std::array<sf::Vector2f, FORCE_VISUALIZATION_RESOLUTION> inner_points;
			float angle;
			float magnitude;
			sf::Vector2f point = sf::Vector2f(0, 0);
			for (int angle_idx = 0; angle_idx < FORCE_VISUALIZATION_RESOLUTION; ++angle_idx)
			{
				angle = (((float)angle_idx) / FORCE_VISUALIZATION_RESOLUTION) * 2 * PI;
				magnitude = visualization_magnitude[angle_idx];
				point.x = (magnitude + radius_map[entity_id].radius) * std::cos(angle);
				point.y = (magnitude + radius_map[entity_id].radius) * std::sin(angle);
				outer_points[angle_idx] = point;
				point.x = (radius_map[entity_id].radius) * std::cos(angle);
				point.y = (radius_map[entity_id].radius) * std::sin(angle);
				inner_points[angle_idx] = point;
			}
			std::array<sf::ConvexShape, FORCE_VISUALIZATION_RESOLUTION> segments;
			for (int angle_idx = 0; angle_idx < FORCE_VISUALIZATION_RESOLUTION; ++angle_idx)
			{
				segments[angle_idx] = sf::ConvexShape(4);
				segments[angle_idx].setPoint(1, outer_points[angle_idx]);
				segments[angle_idx].setPoint(2, outer_points[(angle_idx + 1) % FORCE_VISUALIZATION_RESOLUTION]);
				segments[angle_idx].setPoint(3, inner_points[(angle_idx + 1) % FORCE_VISUALIZATION_RESOLUTION]);
				segments[angle_idx].setPoint(4, inner_points[angle_idx]);
				segments[angle_idx].setPosition(sf::Vector2f(position_map[entity_id].position));
				globals.render_window.draw(segments[angle_idx]);
			}
		}
	}
};