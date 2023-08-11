#include "level.hpp"
#include "components/creation_data.hpp"
#include "components/edit_mode.hpp"
#include "components/goal.hpp"
#include "components/player.hpp"
#include "game.hpp"
#include "utils/math.hpp"
#include "utils/string_parsing.hpp"
#include <functional>
#include <iostream>
#include <optional>
#include <string>

static void OnEntityCreation(ECSScene& scene, Entity entity)
{
	scene.AddComponent<CreationData>(entity)->creation_time = globals.time;
}

Level::Level() :
	entity_creation_observer_(*this, OnEntityCreation)
{
}

LevelState Level::ComputeState()
{
	for (auto& [entity, goal] : GetEntitiesWith<Goal>())
	{
		if (goal->is_goal)
		{
			return COMPLETED;
		}
	}
	if (GetEntitiesWith<DeadPlayer>().size() > 0)
	{
		return FAILED;
	}
	return PLAYING;
}

LevelMode Level::GetMode()
{
	return mode_;
}

void Level::SetMode(LevelMode new_mode)
{
	if (mode_ == EDIT_MODE)
	{
		SaveToFile();
	}
	if (new_mode == EDIT_MODE)
	{
		LoadFromFile();
	}
	mode_ = new_mode;
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

int Level::GetValidNewSizeId(int increment)
{
	return Clamp(grid_size_id + increment, 0, int(LEVEL_SIZES.size() - 1)) - grid_size_id;
}

void Level::ModifyLevelSize(int increment)
{
	grid_size_id += GetValidNewSizeId(increment);
	GetSingleton<EditModeUI>()->initialized = false;
}

Error Level::LoadFromFile()
{
	return LoadFromFile(savefile_path_);
}

void Level::SaveToFile()
{
	SaveToFile(savefile_path_);
}

void Level::DiscardChanges()
{
	LoadFromFile();
	editor.UndoAll();
	SaveToFile();
}
