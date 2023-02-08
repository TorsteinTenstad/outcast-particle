#include "grid_entities_manager.hpp"
#include <iostream>

GridEntitiesData::GridEntitiesData()
{
	data_.fill(0);
}

void GridEntitiesData::Initialize(std::string savefile_identifier)
{
	savefile_identifier_ = savefile_identifier;
	data_texture_.create(WIDTH, HEIGHT);
}

sf::Uint8 GridEntitiesData::GetValue(std::size_t x, std::size_t y, std::size_t channel)
{
	if (x < 0 || y < 0 || channel < 0 || x >= WIDTH || y >= HEIGHT || channel >= 4)
	{
		return 0;
	}
	std::size_t index = (y * WIDTH + x) * 4 + channel;
	return data_[index];
}

void GridEntitiesData::SetValue(std::size_t x, std::size_t y, std::size_t channel, sf::Uint8 value)
{
	if (x < 0 || y < 0 || channel < 0 || x >= WIDTH || y >= HEIGHT || channel >= 4)
	{
		return;
	}
	std::size_t index = (y * WIDTH + x) * 4 + channel;
	data_[index] = value;
	data_texture_.update(data_.data());
}

const sf::Texture* GridEntitiesData::GetDataTexture()
{
	data_texture_.update(data_.data());
	return &data_texture_;
}