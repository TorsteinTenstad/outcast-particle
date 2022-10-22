#include "render_system.hpp"

void RenderSystem::Update(sf::RenderWindow& window, std::map<int, DrawInfo> drawinfo)
{
	for (auto const& [entity_id, entity_drawinfo] : drawinfo)
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
		sprites_[entity_id].setTexture(textures_[entity_drawinfo.image_path]);
		window.draw(sprites_[entity_id]);
	}
}