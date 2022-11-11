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
	std::map<int, sf::Sprite> sprites_;
	std::map<int, sf::RectangleShape> rectangle_shapes_;

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
		std::map<int, Border>& border_map = level.GetComponent<Border>();

		for (auto const& [entity_id, entity_drawinfo] : draw_info_map)
		{
			if (textures_.count(entity_drawinfo.image_path) == 0)
			{
				textures_[entity_drawinfo.image_path] = sf::Texture();
				textures_[entity_drawinfo.image_path].loadFromFile(entity_drawinfo.image_path);
			}
			if (width_and_height_map.count(entity_id))
			{
				if (rectangle_shapes_.count(entity_id) == 0)
				{
					rectangle_shapes_[entity_id] = sf::RectangleShape();
				}
				rectangle_shapes_[entity_id].setSize(width_and_height_map[entity_id].width_and_height);
				rectangle_shapes_[entity_id].setTexture(&textures_[entity_drawinfo.image_path]);
				rectangle_shapes_[entity_id].setOrigin(width_and_height_map[entity_id].width_and_height.x / 2, width_and_height_map[entity_id].width_and_height.y / 2);
				rectangle_shapes_[entity_id].setPosition(position_map[entity_id].position);
				if (border_map.count(entity_id))
				{
					rectangle_shapes_[entity_id].setOutlineThickness(border_map[entity_id].thickness);
					rectangle_shapes_[entity_id].setOutlineColor(border_map[entity_id].color);
				}
				globals.render_window.draw(rectangle_shapes_[entity_id]);
			}
			else
			{
				if (sprites_.count(entity_id) == 0)
				{
					sprites_[entity_id] = sf::Sprite();
				}
				sprites_[entity_id].setOrigin(textures_[entity_drawinfo.image_path].getSize().x / 2, textures_[entity_drawinfo.image_path].getSize().y / 2);
				sprites_[entity_id].setTexture(textures_[entity_drawinfo.image_path]);
				sprites_[entity_id].setPosition(position_map[entity_id].position);
				globals.render_window.draw(sprites_[entity_id]);
			}
		}
	}
};