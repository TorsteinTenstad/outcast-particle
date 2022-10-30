#pragma once
#include "PCH.hpp"
#include "comp_draw_info.hpp"
#include "comp_physics.hpp"
#include "globals.hpp"

class SFMLRenderSystem
{
private:
	std::map<std::string, sf::Texture> textures_;
	std::map<int, sf::Sprite> sprites_;
	std::map<int, sf::RectangleShape> rectangle_shapes_;

public:
	void Update(std::map<int, DrawInfo>& drawinfo_map, std::map<int, Position>& position_map, std::map<int, WidthAndHight>& width_and_hight_map, std::map<int, Boarder>& boarder_map)
	{
		for (auto const& [entity_id, entity_drawinfo] : drawinfo_map)
		{
			if (textures_.count(entity_drawinfo.image_path) == 0)
			{
				textures_[entity_drawinfo.image_path] = sf::Texture();
				textures_[entity_drawinfo.image_path].loadFromFile(entity_drawinfo.image_path);
			}
			if (width_and_hight_map.count(entity_id))
			{
				if (rectangle_shapes_.count(entity_id) == 0)
				{
					rectangle_shapes_[entity_id] = sf::RectangleShape();
				}
				rectangle_shapes_[entity_id].setTexture(&textures_[entity_drawinfo.image_path]);
				rectangle_shapes_[entity_id].setSize(width_and_hight_map[entity_id].width_and_hight);
				rectangle_shapes_[entity_id].setPosition(position_map[entity_id].position);
				if (boarder_map.count(entity_id))
				{
					rectangle_shapes_[entity_id].setOutlineThickness(boarder_map[entity_id].thickness);
					rectangle_shapes_[entity_id].setOutlineColor(boarder_map[entity_id].color);
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