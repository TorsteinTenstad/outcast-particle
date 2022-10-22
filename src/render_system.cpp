#include "render_system.hpp"

void RenderSystem::Update(sf::RenderWindow& window, std::map<int, DrawInfo> drawinfo)
{
	for (auto const& [entity_id, entity_drawinfo] : drawinfo)
	{
		sf::Sprite sprite;
		if (textures_.count(entity_drawinfo.image_path) == 0)
		{
			sf::Texture texture;
			texture.loadFromFile(entity_drawinfo.image_path);
			textures_[entity_drawinfo.image_path] = texture;
		}
		sprite.setTexture(textures_[entity_drawinfo.image_path]);
		window.draw(sprite);
	}
}