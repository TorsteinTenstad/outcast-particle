#pragma once
#include "PCH.hpp"
#include "draw_info.hpp"
#include "physics_components.hpp"

class RenderSystem
{
private:
	std::map<std::string, sf::Texture> textures_;
	std::map<int, sf::Sprite> sprites_;

public:
	void Update(sf::RenderWindow& window, std::map<int, DrawInfo> drawinfo, std::map<int, Position> position);
};