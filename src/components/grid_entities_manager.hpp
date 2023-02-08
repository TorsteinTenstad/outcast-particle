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
	sf::Uint8 GetValue(std::size_t x, std::size_t y, std::size_t channel);
	void SetValue(std::size_t x, std::size_t y, std::size_t channel, sf::Uint8 value);
	const sf::Texture* GetDataTexture();

	friend std::string ToString(const GridEntitiesData& x);
	friend void FromString(GridEntitiesData& x, std::string& str);
};

enum GridEntity
{
	EMPTY = 0,
	WALL,
	LASER
};

class GridEntitiesManager
{
public:
	GridEntitiesData grid_entities_data;
	// [NoSerialize]
	bool initialized = false;
	// [NoSerialize]
	std::vector<int> physics_entities;
};

class GridEntitiesManagerRuntimeData
{
public:
};
