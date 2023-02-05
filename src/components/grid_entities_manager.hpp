#pragma once
#include "constants.hpp"
#include <SFML/Graphics.hpp>
#include <array>
#include <cstddef>

class GridEntitiesData
{
public:
	static constexpr std::size_t WIDTH = 64;
	static constexpr std::size_t HEIGHT = 36;

private:
	sf::Texture data_texture_;
	std::array<sf::Uint8, WIDTH * HEIGHT * 4> data_;
	std::string savefile_identifier_;

public:
	GridEntitiesData();
	void Initialize(std::string savefile_identifier);
	void SetValue(std::size_t x, std::size_t y, std::size_t channel, sf::Uint8 value);
	const sf::Texture* GetDataTexture();

	friend std::string ToString(const GridEntitiesData& x);
	friend void FromString(GridEntitiesData& x, std::string& str);
};

enum GridEntity
{
	EMPTY = 0,
	WALL,
	BOUNCE_WALL,
	NO_BOUNCE_WALL
};

class GridEntitiesManager
{
public:
	// [NoSerialize]
	bool initialized = false;
	GridEntitiesData grid_entities_data;
};

class GridEntitiesManagerRuntimeData
{
public:
};
