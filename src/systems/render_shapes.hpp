#pragma once
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "components/position.hpp"
#include "game_system.hpp"
#include "level.hpp"

class RenderShapesSystem : public GameSystem
{
private:
	std::map<std::string, sf::Texture> textures_;
	std::map<int, sf::RectangleShape> rectangle_shapes_;
	std::map<int, sf::CircleShape> circle_shapes_;

	sf::Texture* GetLoadedSFMLTexture(std::string image_path);
	EntityBoundDrawable RenderShapesSystem::RenderShape(int entity_id, sf::Shape* shape, sf::Texture* texture, sf::Vector2f w_h, sf::Color fill_color, Position* position);

public:
	using GameSystem::GameSystem;
	sf::Texture* RegisterTexture(std::string identifier);
	void Update(Level& level, float dt);
};