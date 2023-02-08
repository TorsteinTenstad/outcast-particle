#pragma once
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include <cmath>
#include <tuple>
#include <vector>

#define MAX_COLS 32
#define MAX_ROWS 18

static sf::Vector2i WorldPosToGridPos(sf::Vector2f world_pos, float grid_size)
{
	sf::Vector2f unrounded_grid_pos = world_pos / grid_size;
	return sf::Vector2i((int)std::floor(unrounded_grid_pos.x), (int)std::floor(unrounded_grid_pos.y));
}

static void RefreshPhysicsEntities(Level& level, GridEntitiesManager* grid_entities_manager)
{
	for (auto entity_id : grid_entities_manager->physics_entities)
	{
		level.DeleteEntity(entity_id);
	}
	grid_entities_manager->physics_entities.clear();
	for (int i = 0; i < grid_entities_manager->grid_entities_data.WIDTH; i++)
	{
		for (int j = 0; j < grid_entities_manager->grid_entities_data.HEIGHT; j++)
		{
			switch (grid_entities_manager->grid_entities_data.GetValue(i, j, 0))
			{
				case EMPTY:
					break;
				case WALL: {
					auto [entity_id, collision, width_and_height, position, sound_info] = level.CreateEntitiyWith<Collision, WidthAndHeight, Position, SoundInfo>();
					grid_entities_manager->physics_entities.push_back(entity_id);
					switch (grid_entities_manager->grid_entities_data.GetValue(i, j, 1))
					{
						case 0:
							collision->bounce_factor = 0.2;
							break;
						case 1:
							collision->bounce_factor = 1;
							break;

						default:
							assert(false);
							break;
					}
					collision->friction = 75;
					width_and_height->width_and_height = sf::Vector2f(1, 1) * float(BLOCK_SIZE);
					position->position = sf::Vector2f(i + 0.5, j + 0.5) * float(BLOCK_SIZE);
					sound_info->sound_path = "content\\sounds\\thud.wav";
				}
				break;
				case LASER: {
					auto [entity_id, kill_on_intersection, radius, position, sound_info] = level.CreateEntitiyWith<KillOnIntersection, Radius, Position, SoundInfo>();
					grid_entities_manager->physics_entities.push_back(entity_id);
					radius->radius = 0.25 * float(BLOCK_SIZE);
					position->position = sf::Vector2f(i + 0.5, j + 0.5) * float(BLOCK_SIZE);
					sound_info->sound_path = "content\\sounds\\laser.wav";
				}
				break;

				default:
					assert(false);
					break;
			}
		}
	}
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
		if (IsMenu(level.GetId()))
		{
			return;
		}
		if (level.GetMode() == EDIT_MODE && level.GetEntitiesWith<GridEntitiesManager>().size() == 0)
		{
			int id = level.AddBlueprint("BPGridEntitiesManager");
		}
		for (auto [entity_id, grid_entities_manager, draw_info, shader, width_and_height, position] : level.GetEntitiesWith<GridEntitiesManager, DrawInfo, Shader, WidthAndHeight, Position>())
		{
			sf::Vector2u level_size = level.GetGridSize();
			shader->int_uniforms["level_width"] = level_size.x;
			shader->int_uniforms["level_height"] = level_size.y;
			if (!grid_entities_manager->initialized)
			{
				grid_entities_manager->initialized = true;
				sf::Vector2f size = sf::Vector2f(GridEntitiesData::WIDTH, GridEntitiesData::HEIGHT) * (float)BLOCK_SIZE;
				width_and_height->width_and_height = size;
				position->position = size / 2.f;

				grid_entities_manager->grid_entities_data.Initialize(GetLevelDisplayNameFromId(active_level_id_));

				shader->tex_uniforms["data_texture"] = grid_entities_manager->grid_entities_data.GetDataTexture();
				shader->int_uniforms["TEXTURE_WIDTH"] = grid_entities_manager->grid_entities_data.WIDTH;
				shader->int_uniforms["TEXTURE_HEIGHT"] = grid_entities_manager->grid_entities_data.HEIGHT;
				shader->int_uniforms["BLOCK_SIZE"] = BLOCK_SIZE;

				RefreshPhysicsEntities(level, grid_entities_manager);
			}
			if (level.GetMode() == EDIT_MODE)
			{
				sf::Vector2f level_size = level.GetSize();
				if (level_size.x < 0 || level_size.y < 0 || cursor_and_keys_.cursor_position.x > level_size.x || cursor_and_keys_.cursor_position.y > level_size.y)
				{
					return;
				}
				sf::Vector2i mouse_grid_pos = WorldPosToGridPos(cursor_and_keys_.cursor_position, BLOCK_SIZE);
				std::vector<std::tuple<sf::Mouse::Button, GridEntity, int>> a;
				if (cursor_and_keys_.key_down[sf::Keyboard::Num1])
				{
					a.push_back(std::make_tuple(sf::Mouse::Button::Left, WALL, 0));
				}
				if (cursor_and_keys_.key_down[sf::Keyboard::Num2])
				{
					a.push_back(std::make_tuple(sf::Mouse::Button::Left, LASER, 0));
				}
				if (cursor_and_keys_.key_down[sf::Keyboard::Num3])
				{
					a.push_back(std::make_tuple(sf::Mouse::Button::Left, WALL, 1));
				}
				a.push_back(std::make_tuple(sf::Mouse::Button::Right, EMPTY, 0));
				for (auto& [button, grid_entity, variant] : a)
				{
					if (cursor_and_keys_.mouse_button_down[button]
						&& (grid_entities_manager->grid_entities_data.GetValue(mouse_grid_pos.x, mouse_grid_pos.y, 0) != grid_entity
							|| grid_entities_manager->grid_entities_data.GetValue(mouse_grid_pos.x, mouse_grid_pos.y, 1) != variant))
					{
						grid_entities_manager->grid_entities_data.SetValue(mouse_grid_pos.x, mouse_grid_pos.y, 0, grid_entity);
						grid_entities_manager->grid_entities_data.SetValue(mouse_grid_pos.x, mouse_grid_pos.y, 1, variant);
						RefreshPhysicsEntities(level, grid_entities_manager);
					}
				}
			}
		}
	}
};