#pragma once
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "utils.hpp"

class EditModeSystem : public GameSystem
{
private:
	using GameSystem::GameSystem;
	const float default_velocity_magnitude_change_sensitivity_ = 400;
	const float default_velocity_angle_change_sensitivity_ = PI / 2;
	const std::vector<std::string> blueprint_menu_entry_tags_ { "BPStaticParticle", "BPPlayer", "BPLaser", "BPWall", "BPGoal", "BPElectricField", "BPMagneticField", "BPCoin" };

	bool blueprint_menu_is_open = false;

public:
	void Update(Level& level, float dt)
	{
		if (mode_ != EDIT_MODE)
		{
			return;
		}
		auto& position_map = level.GetComponent<Position>();
		auto& velocity_map = level.GetComponent<Velocity>();
		auto& editable_map = level.GetComponent<Editable>();
		auto& blueprint_menu_item_map = level.GetComponent<BlueprintMenuItem>();
		auto& clicked_on_map = level.GetComponent<ClickedOn>();
		auto& width_and_height_map = level.GetComponent<WidthAndHeight>();
		auto& border_map = level.GetComponent<Border>();
		auto& draw_info_map = level.GetComponent<DrawInfo>();
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& charge_map = level.GetComponent<Charge>();
		auto& electric_field_map = level.GetComponent<ElectricField>();
		auto& magnetic_field_map = level.GetComponent<MagneticField>();

		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::B])
		{
			if (blueprint_menu_is_open)
			{
				CloseBlueprintMenu(level);
			}
			else
			{
				OpenBlueprintMenu(level);
			}
		}

		// Change level size:
		int size_increase = 0;
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.INCREASE_LEVEL_SIZE])
		{
			size_increase += 1;
		}
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DECREASE_LEVEL_SIZE])
		{
			size_increase -= 1;
		}
		if (size_increase != 0)
		{
			int width_idx = FindClosest(LEVEL_WIDTHS, level.size.x) + size_increase;
			width_idx = Clamp(width_idx, 0, (int)LEVEL_WIDTHS.size() - 1);
			float new_level_width = LEVEL_WIDTHS[width_idx];
			level.size.x = new_level_width;
			level.size.y = new_level_width / ASPECT_RATIO;
		}

		// Copy entities:
		for (auto& [entity_id, editable_entity] : editable_map)
		{
			if (editable_entity.selected && cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY] && cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left])
			{
				int new_id = level.CopyEntity(entity_id);
				editable_map[new_id].selected = false;
				position_map[new_id].position = cursor_and_keys_.cursor_position - editable_entity.drag_offset;
				position_map[new_id].position.x -= std::fmod(position_map[new_id].position.x, BLOCK_SIZE / 2);
				position_map[new_id].position.y -= std::fmod(position_map[new_id].position.y, BLOCK_SIZE / 2);
			}
		}

		// Select/Deselect entities:
		for (auto& [entity_id, editable_entity] : editable_map)
		{
			if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left])
			{
				if (!cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY])
				{
					editable_entity.drag_offset = cursor_and_keys_.cursor_position - position_map[entity_id].position;
				}
				if (!(cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES] || cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY]))
				{
					editable_entity.selected = false;
				}
			}
			if (clicked_on_map[entity_id].clicked_this_frame)
			{
				editable_entity.selected = true;
			}
			if (editable_entity.selected)
			{
				border_map[entity_id];
			}
			else
			{
				border_map.erase(entity_id);
			}
		}

		// Handle selection of entity in blueprint menu:
		for (auto& [entity_id, blueprint_menu_item] : blueprint_menu_item_map)
		{
			if (editable_map.count(entity_id) > 0 && editable_map[entity_id].selected)
			{
				blueprint_menu_item_map.erase(entity_id);
				draw_priority_map[entity_id].draw_priority -= UI_BASE_DRAW_PRIORITY;
				CloseBlueprintMenu(level);
				break;
			}
		}

		// Edit entities:
		for (auto it = editable_map.cbegin(), next_it = it; it != editable_map.cend(); it = next_it)
		{
			++next_it;
			auto entity_id = it->first;
			auto editable_entity = it->second;

			bool snap_to_grid = false;
			// Delete entities:
			if (editable_entity.selected && cursor_and_keys_.key_pressed_this_frame[globals.key_config.DELETE_ENTITY])
			{
				level.DeleteEntity(entity_id);
			}

			// Move entities with the curser:
			if (editable_entity.selected && editable_entity.is_position_editable && cursor_and_keys_.mouse_button_down[sf::Mouse::Left] && !cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY])
			{
				snap_to_grid = true;
				position_map[entity_id].position = cursor_and_keys_.cursor_position - editable_entity.drag_offset;
			}

			// Edit charge:
			if (editable_entity.selected && editable_entity.is_charge_editable)
			{
				if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.EDIT_MODE_SWITCH_CHARGE])
				{
					charge_map[entity_id].charge = -charge_map[entity_id].charge;
				}
				for (unsigned i = 0; i < CHARGE_CATEGORY_KEYS.size(); ++i)
				{
					if (cursor_and_keys_.key_pressed_this_frame[CHARGE_CATEGORY_KEYS[i]])
					{
						charge_map[entity_id].charge = Sign(charge_map[entity_id].charge) * abs(PARTICLE_CHARGE_CATEGORIES[i]);
					}
				}
			}

			// Edit velocity:
			if (editable_entity.selected && editable_entity.is_velocity_editable)
			{
				float velocity_magnitude = Magnitude(velocity_map[entity_id].velocity);
				float velocity_angle = Angle(velocity_map[entity_id].velocity);

				if (cursor_and_keys_.key_down[globals.key_config.INCREMENT_VELOCITY])
				{
					velocity_magnitude += default_velocity_magnitude_change_sensitivity_ * dt / (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
				}
				if (cursor_and_keys_.key_down[globals.key_config.DECREMENT_VELOCITY])
				{
					velocity_magnitude -= default_velocity_magnitude_change_sensitivity_ * dt / (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
				}
				if (cursor_and_keys_.key_down[globals.key_config.INCREMENT_VELOCITY_ANGLE])
				{
					velocity_angle += default_velocity_angle_change_sensitivity_ * dt / (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
				}
				if (cursor_and_keys_.key_down[globals.key_config.DECREMENT_VELOCITY_ANGLE])
				{
					velocity_angle -= default_velocity_angle_change_sensitivity_ * dt / (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
				}
				velocity_map[entity_id].velocity.x = velocity_magnitude * std::cos(velocity_angle);
				velocity_map[entity_id].velocity.y = velocity_magnitude * std::sin(velocity_angle);
			}

			// Edit magnetic field:
			if (editable_entity.selected && magnetic_field_map.count(entity_id) > 0)
			{
				if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.EDIT_MODE_SWITCH_MAGNETIC_FIELD_DIRECTION])
				{
					magnetic_field_map[entity_id].field_strength *= -1;
				}
				for (unsigned i = 0; i < MAGNETIC_FIELD_CATEGORY_KEYS.size(); ++i)
				{
					if (cursor_and_keys_.key_pressed_this_frame[MAGNETIC_FIELD_CATEGORY_KEYS[i]])
					{
						magnetic_field_map[entity_id].field_strength = Sign(magnetic_field_map[entity_id].field_strength) * abs(MAGNETIC_FIELD_STRENGTH_CATEGORIES[i]);
					}
				}
			}

			// Edit electric field:
			if (editable_entity.selected && electric_field_map.count(entity_id) > 0)
			{
				for (unsigned i = 0; i < ELECTRIC_FIELD_CATEGORY_KEYS.size(); ++i)
				{
					if (cursor_and_keys_.key_pressed_this_frame[ELECTRIC_FIELD_CATEGORY_KEYS[i]])
					{
						electric_field_map[entity_id].field_vector = Normalized(electric_field_map[entity_id].field_vector) * abs(ELECTRIC_FIELD_STRENGTH_CATEGORIES[i]);
					}
				}
			}

			// Edit width, height and rotation of all selected entites with editable width and height:
			if (editable_entity.selected && editable_entity.width_and_height_edit > 0 && width_and_height_map.count(entity_id) > 0)
			{
				if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.ROTATE_ENTITY])
				{
					width_and_height_map[entity_id].width_and_height = sf::Vector2f(width_and_height_map[entity_id].width_and_height.y, width_and_height_map[entity_id].width_and_height.x);
					if (electric_field_map.count(entity_id))
					{
						electric_field_map[entity_id].field_vector = GetQuarterTurnRotation(electric_field_map[entity_id].field_vector);
					}
				}
				if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.INCREMENT_HEIGHT])
				{
					snap_to_grid = true;
					width_and_height_map[entity_id].width_and_height.y += editable_entity.width_and_height_edit * (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
				}
				if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DECREMENT_HEIGHT])
				{
					snap_to_grid = true;
					width_and_height_map[entity_id].width_and_height.y -= editable_entity.width_and_height_edit * (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
					if (width_and_height_map[entity_id].width_and_height.y < editable_entity.width_and_height_edit)
					{
						width_and_height_map[entity_id].width_and_height.y = editable_entity.width_and_height_edit;
					}
				}
				if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.INCREMENT_WIDTH])
				{
					snap_to_grid = true;
					width_and_height_map[entity_id].width_and_height.x += editable_entity.width_and_height_edit * (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
				}
				if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DECREMENT_WIDTH])
				{
					snap_to_grid = true;
					width_and_height_map[entity_id].width_and_height.x -= editable_entity.width_and_height_edit * (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
					if (width_and_height_map[entity_id].width_and_height.x < editable_entity.width_and_height_edit)
					{
						width_and_height_map[entity_id].width_and_height.x = editable_entity.width_and_height_edit;
					}
				}
			}

			//Limit position:
			if (position_map[entity_id].position.x < 0)
			{
				position_map[entity_id].position.x = 0;
			}
			if (position_map[entity_id].position.y < 0)
			{
				position_map[entity_id].position.y = 0;
			}

			//Snap to grid:
			if (snap_to_grid && !cursor_and_keys_.key_down[globals.key_config.SNAP_TO_GRID])
			{
				position_map[entity_id].position.x -= std::fmod(position_map[entity_id].position.x, BLOCK_SIZE / 2);
				position_map[entity_id].position.y -= std::fmod(position_map[entity_id].position.y, BLOCK_SIZE / 2);
			}
		}
	}
	void OpenBlueprintMenu(Level& level)
	{
		int i = 0;
		int menu_background_id = level.CreateEntityId();
		level.GetComponent<Position>()[menu_background_id].position = level.size / 2.f;
		level.GetComponent<DrawInfo>()[menu_background_id].image_path = "content\\textures\\gray.png";
		level.GetComponent<DrawPriority>()[menu_background_id].draw_priority = UI_BASE_DRAW_PRIORITY;
		level.GetComponent<ClickedOn>()[menu_background_id];
		float menu_width = (3 * blueprint_menu_entry_tags_.size() + 1) * BLOCK_SIZE;
		level.GetComponent<WidthAndHeight>()[menu_background_id].width_and_height = sf::Vector2f(menu_width, 4 * BLOCK_SIZE);
		level.GetComponent<Border>()[menu_background_id];
		level.GetComponent<BlueprintMenuItem>()[menu_background_id];
		int entity_id;
		for (const auto& tag : blueprint_menu_entry_tags_)
		{
			entity_id = level.AddBlueprint(tag);
			level.GetComponent<Position>()[entity_id].position = sf::Vector2f(level.size.x / 2 - menu_width / 2 + (2 + 3 * i) * BLOCK_SIZE, level.size.y / 2);
			level.GetComponent<DrawPriority>()[entity_id].draw_priority += UI_BASE_DRAW_PRIORITY;
			level.GetComponent<BlueprintMenuItem>()[entity_id];
			i++;
		}
		blueprint_menu_is_open = true;
	}
	void CloseBlueprintMenu(Level& level)
	{
		for (auto it = level.GetComponent<BlueprintMenuItem>().cbegin(), next_it = it; it != level.GetComponent<BlueprintMenuItem>().cend(); it = next_it)
		{
			++next_it;
			auto entity_id = it->first;
			level.DeleteEntity(entity_id);
		}
		blueprint_menu_is_open = false;
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};