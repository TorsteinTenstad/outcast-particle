#include "level.hpp"
#include "game.hpp"
#include "string_parsing_utils.hpp"
#include <functional>
#include <iostream>
#include <optional>
#include <string>

const std::array<sf::Vector2u, 5> LEVEL_SIZES { { sf::Vector2u(16, 9), sf::Vector2u(32, 18), sf::Vector2u(48, 27), sf::Vector2u(64, 36), sf::Vector2u(80, 45) } };

LevelState Level::ComputeState()
{
	for (auto& [entity_id, goal] : GetEntitiesWith<Goal>())
	{
		if (goal->is_goal)
		{
			return COMPLETED;
		}
	}
	if (GetEntitiesWith<Player>().size() == 0)
	{
		return FAILED;
	}
	return PLAYING;
}

LevelMode Level::GetMode()
{
	return mode_;
}

void Level::SetMode(LevelMode level_mode)
{
	if (mode_ == EDIT_MODE)
	{
		SaveToFile();
		LoadFromFile();
	}
	if (level_mode == EDIT_MODE)
	{
		LoadFromFile();
	}
	mode_ = level_mode;
}

sf::Vector2f Level::GetSize()
{
	return sf::Vector2f(LEVEL_SIZES[grid_size_id]) * float(BLOCK_SIZE);
}

sf::Vector2u Level::GetGridSize()
{
	return LEVEL_SIZES[grid_size_id];
}

float Level::GetScale()
{
	return (float)LEVEL_SIZES[grid_size_id].x / (float)LEVEL_SIZES[DEFAULT_LEVEL_GRID_SIZE_ID].x;
}

void Level::ResetSize()
{
	grid_size_id = DEFAULT_LEVEL_GRID_SIZE_ID;
}

void Level::IncreaseSize()
{
	if (grid_size_id < LEVEL_SIZES.size() - 1)
	{
		grid_size_id++;
	}
}

void Level::DecreaseSize()
{
	if (grid_size_id > 0)
	{
		grid_size_id--;
	}
}

void Level::LoadFromFile()
{
	LoadFromFile(savefile_path_);
}

void Level::SaveToFile()
{
	SaveToFile(savefile_path_);
}

