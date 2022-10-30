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

public:
	void Update(std::map<int, DrawInfo>& drawinfo_map, std::map<int, Position>& position_map)
	{
		for (auto const& [entity_id, entity_drawinfo] : drawinfo_map)
		{
			if (sprites_.count(entity_id) == 0)
			{
				sprites_[entity_id] = sf::Sprite();
			}
			if (textures_.count(entity_drawinfo.image_path) == 0)
			{
				textures_[entity_drawinfo.image_path] = sf::Texture();
				textures_[entity_drawinfo.image_path].loadFromFile(entity_drawinfo.image_path);
			}
			sprites_[entity_id].setOrigin(textures_[entity_drawinfo.image_path].getSize().x / 2, textures_[entity_drawinfo.image_path].getSize().y / 2);
			sprites_[entity_id].setTexture(textures_[entity_drawinfo.image_path]);
			sprites_[entity_id].setPosition(position_map[entity_id].position);
			globals.render_window.draw(sprites_[entity_id]);
		}
	}
};