#pragma once
#include "level.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>

class GridEntitiesDataX
{
public:
	static constexpr std::size_t WIDTH = 64;
	static constexpr std::size_t HEIGHT = 36;

private:
	std::array<sf::Uint8, WIDTH * HEIGHT * 4> data_;

public:
	GridEntitiesDataX();

	void SetValue(std::size_t x, std::size_t y, std::size_t channel, sf::Uint8 value);

	void UpdateSFMLTexture(sf::Texture* texture) const;
};
