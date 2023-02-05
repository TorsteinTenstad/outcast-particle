#pragma once
#include "SFML/Window/Mouse.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include <cmath>

#define MAX_COLS 32
#define MAX_ROWS 18

static sf::Vector2i WorldPosToGridPos(sf::Vector2f world_pos, float grid_size)
{
	sf::Vector2f unrounded_grid_pos = world_pos / grid_size;
	return sf::Vector2i((int)std::floor(unrounded_grid_pos.x), (int)std::floor(unrounded_grid_pos.y));
}

class GridEntitiesManagerSystem : public GameSystem
{
private:
	std::function<sf::Texture*(std::string)> register_data_texture_;

public:
	using GameSystem::GameSystem;
	void Give(std::function<sf::Texture*(std::string)> register_data_texture)
	{
		register_data_texture_ = register_data_texture;
	}
	void Update(Level& level, float dt)
	{
		if (IsMenu(active_level_id_))
		{
			return;
		}
		if (level.GetMode() == EDIT_MODE && level.GetEntitiesWith<GridEntitiesManager>().size() == 0)
		{
			int id = level.AddBlueprint("BPGridEntitiesManager");
		}
		for (auto [entity_id, grid_entities_manager, draw_info, shader, width_and_height, position] : level.GetEntitiesWith<GridEntitiesManager, DrawInfo, Shader, WidthAndHeight, Position>())
		{
			if (!grid_entities_manager->initialized)
			{
				grid_entities_manager->initialized = true;
				sf::Vector2f size = sf::Vector2f(GridEntitiesData::WIDTH, GridEntitiesData::HEIGHT) * (float)BLOCK_SIZE;
				width_and_height->width_and_height = size;
				position->position = size / 2.f;

				grid_entities_manager->grid_entities_data.Initialize(GetLevelDisplayNameFromId(active_level_id_));

				shader->tex_uniforms["data_texture"] = grid_entities_manager->grid_entities_data.GetDataTexture();
				shader->int_uniforms["WIDTH"] = grid_entities_manager->grid_entities_data.WIDTH;
				shader->int_uniforms["HEIGHT"] = grid_entities_manager->grid_entities_data.HEIGHT;
				shader->int_uniforms["BLOCK_SIZE"] = BLOCK_SIZE;
			}

			sf::Vector2i mouse_grid_pos = WorldPosToGridPos(cursor_and_keys_.cursor_position, BLOCK_SIZE);
			if (level.GetMode() == EDIT_MODE && cursor_and_keys_.mouse_button_down[sf::Mouse::Button::Left])
			{
				grid_entities_manager->grid_entities_data.SetValue(mouse_grid_pos.x, mouse_grid_pos.y, 0, WALL);
			}
		}
	}
};