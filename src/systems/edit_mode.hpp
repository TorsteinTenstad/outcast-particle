#pragma once
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "utilityfunctions.hpp"

class EditModeSystem : public GameSystem
{
private:
	const float default_velocity_magnitude_change_sensitivity_ = 400;
	const float default_velocity_angle_change_sensitivity_ = PI / 2;
	const std::vector<std::string> blueprint_menu_entry_tags_ { "BPStaticParticle", "BPPlayer", "BPLaser", "BPWall", "BPGoal", "BPElectricField", "BPMagneticField" };

	bool blueprint_menu_is_open = false;

public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)dt;
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, Velocity>& velocity_map = level.GetComponent<Velocity>();
		std::map<int, Editable>& editable_map = level.GetComponent<Editable>();
		std::map<int, BlueprintMenuItem>& blueprint_menu_item_map = level.GetComponent<BlueprintMenuItem>();
		std::map<int, ClickedOn>& clicked_on_map = level.GetComponent<ClickedOn>();
		std::map<int, WidthAndHeight>& width_and_height_map = level.GetComponent<WidthAndHeight>();
		std::map<int, Border>& border_map = level.GetComponent<Border>();
		std::map<int, DrawInfo>& draw_info_map = level.GetComponent<DrawInfo>();
		std::map<int, Charge>& charge_map = level.GetComponent<Charge>();
		std::map<int, ElectricField>& electric_field_map = level.GetComponent<ElectricField>();
		std::map<int, MagneticField>& magnetic_field_map = level.GetComponent<MagneticField>();

		if (level.editable && cursor_and_keys.key_pressed_this_frame[EDIT_MODE_KEY])
		{
			if (level.edit_mode)
			{
				level.SaveToFile();
				for (auto& [entity_id, editable_entity] : editable_map)
				{
					border_map.erase(entity_id);
				}
			}
			else
			{
				level.LoadFromFile();
			}
			level.edit_mode = !level.edit_mode;
		}
		if (!level.edit_mode)
		{
			CloseBlueprintMenu(level);
			return;
		}
		if (cursor_and_keys.key_pressed_this_frame[sf::Keyboard::B])
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
		if (cursor_and_keys.key_pressed_this_frame[INCREASE_LEVEL_SIZE_KEY])
		{
			level.size += sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE * 9 / 16);
		}
		if (cursor_and_keys.key_pressed_this_frame[DECREASE_LEVEL_SIZE_KEY])
		{
			level.size -= sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE * 9 / 16);
		}

		// Copy entities:
		for (auto& [entity_id, editable_entity] : editable_map)
		{
			if (editable_entity.selected && cursor_and_keys.key_down[COPY_ENTITY_KEY] && cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left])
			{
				int new_id = level.CopyEntity(entity_id);
				position_map[new_id].position = cursor_and_keys.cursor_position - editable_entity.drag_offset;
				editable_map[new_id].selected = false;
				if (!cursor_and_keys.key_down[SNAP_TO_GRID_KEY])
				{
					position_map[new_id].position.x -= std::fmod(position_map[new_id].position.x, BLOCK_SIZE / 2);
					position_map[new_id].position.y -= std::fmod(position_map[new_id].position.y, BLOCK_SIZE / 2);
				}
			}
		}

		// Select/Deselect entities:
		for (auto& [entity_id, editable_entity] : editable_map)
		{
			if (cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left])
			{
				if (!cursor_and_keys.key_down[COPY_ENTITY_KEY])
				{
					editable_entity.drag_offset = cursor_and_keys.cursor_position - position_map[entity_id].position;
				}
				if (clicked_on_map[entity_id].clicked_this_frame)
				{
					editable_entity.selected = true;
					if (blueprint_menu_item_map.count(entity_id) > 0)
					{
						blueprint_menu_item_map.erase(entity_id);
						draw_info_map[entity_id].draw_priority -= 100;
						CloseBlueprintMenu(level);
					}
				}
				else if (!(cursor_and_keys.key_down[SELECT_MULTIPLE_ENTITIES_KEY] || cursor_and_keys.key_down[COPY_ENTITY_KEY]))
				{
					editable_entity.selected = false;
				}
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

		// Edit entities:
		for (auto it = editable_map.cbegin(), next_it = it; it != editable_map.cend(); it = next_it)
		{
			++next_it;
			auto entity_id = it->first;
			auto editable_entity = it->second;

			bool snap_to_grid = false;
			// Delete entities:
			if (editable_entity.selected && cursor_and_keys.key_pressed_this_frame[DELETE_ENTITY_KEY])
			{
				level.DeleteEntity(entity_id);
			}

			// Move entities with the curser:
			if (editable_entity.selected && editable_entity.is_position_editable && cursor_and_keys.mouse_button_down[sf::Mouse::Left] && !cursor_and_keys.key_down[COPY_ENTITY_KEY])
			{
				snap_to_grid = true;
				position_map[entity_id].position = cursor_and_keys.cursor_position - editable_entity.drag_offset;
			}

			// Edit charge:
			if (editable_entity.selected && editable_entity.is_charge_editable)
			{
				if (cursor_and_keys.key_pressed_this_frame[EDIT_MODE_SWITCH_CHARGE_KEY])
				{
					charge_map[entity_id].charge = -charge_map[entity_id].charge;
				}
				for (unsigned i = 0; i < CHARGE_CATEGORY_KEYS.size(); ++i)
				{
					if (cursor_and_keys.key_pressed_this_frame[CHARGE_CATEGORY_KEYS[i]])
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

				if (cursor_and_keys.key_down[INCREMENT_VELOCITY_KEY])
				{
					velocity_magnitude += default_velocity_magnitude_change_sensitivity_ * dt / (cursor_and_keys.key_down[ALT_SENSITIVITY_KEY] ? 4 : 1);
				}
				if (cursor_and_keys.key_down[DECREMENT_VELOCITY_KEY])
				{
					velocity_magnitude -= default_velocity_magnitude_change_sensitivity_ * dt / (cursor_and_keys.key_down[ALT_SENSITIVITY_KEY] ? 4 : 1);
				}
				if (cursor_and_keys.key_down[INCREMENT_VELOCITY_ANGLE_KEY])
				{
					velocity_angle += default_velocity_angle_change_sensitivity_ * dt / (cursor_and_keys.key_down[ALT_SENSITIVITY_KEY] ? 4 : 1);
				}
				if (cursor_and_keys.key_down[DECREMENT_VELOCITY_ANGLE_KEY])
				{
					velocity_angle -= default_velocity_angle_change_sensitivity_ * dt / (cursor_and_keys.key_down[ALT_SENSITIVITY_KEY] ? 4 : 1);
				}
				velocity_map[entity_id].velocity.x = velocity_magnitude * std::cos(velocity_angle);
				velocity_map[entity_id].velocity.y = velocity_magnitude * std::sin(velocity_angle);
			}

			// Edit magnetic field:
			if (editable_entity.selected && magnetic_field_map.count(entity_id) > 0)
			{
				if (cursor_and_keys.key_pressed_this_frame[EDIT_MODE_SWITCH_MAGNETIC_FIELD_DIRECTION_KEY])
				{
					magnetic_field_map[entity_id].field_strength *= -1;
				}
			}

			// Edit electric field:
			if (editable_entity.selected && electric_field_map.count(entity_id) > 0)
			{
				for (unsigned i = 0; i < ELECTRIC_FIELD_CATEGORY_KEYS.size(); ++i)
				{
					if (cursor_and_keys.key_pressed_this_frame[ELECTRIC_FIELD_CATEGORY_KEYS[i]])
					{
						electric_field_map[entity_id].field_vector = Normalized(electric_field_map[entity_id].field_vector) * abs(ELECTRIC_FIELD_STRENGTH_CATEGORIES[i]);
					}
				}
			}

			// Edit width, height and rotation of all selected entites with editable width and height:
			if (editable_entity.selected && editable_entity.width_and_height_edit > 0 && width_and_height_map.count(entity_id) > 0)
			{
				if (cursor_and_keys.key_pressed_this_frame[ROTATE_ENTITY_KEY])
				{
					width_and_height_map[entity_id].width_and_height = sf::Vector2f(width_and_height_map[entity_id].width_and_height.y, width_and_height_map[entity_id].width_and_height.x);
					if (electric_field_map.count(entity_id))
					{
						electric_field_map[entity_id].field_vector = GetQuarterTurnRotation(electric_field_map[entity_id].field_vector);
					}
				}
				if (cursor_and_keys.key_pressed_this_frame[INCREMENT_HEIGHT_KEY])
				{
					snap_to_grid = true;
					width_and_height_map[entity_id].width_and_height.y += editable_entity.width_and_height_edit * (cursor_and_keys.key_down[ALT_SENSITIVITY_KEY] ? 4 : 1);
				}
				if (cursor_and_keys.key_pressed_this_frame[DECREMENT_HEIGHT_KEY])
				{
					snap_to_grid = true;
					width_and_height_map[entity_id].width_and_height.y -= editable_entity.width_and_height_edit * (cursor_and_keys.key_down[ALT_SENSITIVITY_KEY] ? 4 : 1);
					if (width_and_height_map[entity_id].width_and_height.y < editable_entity.width_and_height_edit)
					{
						width_and_height_map[entity_id].width_and_height.y = editable_entity.width_and_height_edit;
					}
				}
				if (cursor_and_keys.key_pressed_this_frame[INCREMENT_WIDTH_KEY])
				{
					snap_to_grid = true;
					width_and_height_map[entity_id].width_and_height.x += editable_entity.width_and_height_edit * (cursor_and_keys.key_down[ALT_SENSITIVITY_KEY] ? 4 : 1);
				}
				if (cursor_and_keys.key_pressed_this_frame[DECREMENT_WIDTH_KEY])
				{
					snap_to_grid = true;
					width_and_height_map[entity_id].width_and_height.x -= editable_entity.width_and_height_edit * (cursor_and_keys.key_down[ALT_SENSITIVITY_KEY] ? 4 : 1);
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
			if (snap_to_grid && !cursor_and_keys.key_down[SNAP_TO_GRID_KEY])
			{
				position_map[entity_id].position.x -= std::fmod(position_map[entity_id].position.x, BLOCK_SIZE / 2);
				position_map[entity_id].position.y -= std::fmod(position_map[entity_id].position.y, BLOCK_SIZE / 2);
			}
		}
	}
	void OpenBlueprintMenu(Level& level)
	{
		int entity_id;
		int i = 0;
		int menu_background_id = level.CreateEntityId();
		level.GetComponent<Position>()[menu_background_id].position = level.size / 2.f;
		level.GetComponent<DrawInfo>()[menu_background_id].image_path = "content\\textures\\gray.png";
		level.GetComponent<DrawInfo>()[menu_background_id].draw_priority = 50;
		level.GetComponent<ClickedOn>()[menu_background_id];
		float menu_width = (3 * blueprint_menu_entry_tags_.size() + 1) * BLOCK_SIZE;
		level.GetComponent<WidthAndHeight>()[menu_background_id].width_and_height = sf::Vector2f(menu_width, 4 * BLOCK_SIZE);
		level.GetComponent<Border>()[menu_background_id];
		level.GetComponent<BlueprintMenuItem>()[menu_background_id];
		for (const auto& tag : blueprint_menu_entry_tags_)
		{
			entity_id = level.AddBlueprint(tag);
			level.GetComponent<Position>()[entity_id].position = sf::Vector2f(level.size.x / 2 - menu_width / 2 + (2 + 3 * i) * BLOCK_SIZE, level.size.y / 2);
			level.GetComponent<DrawInfo>()[entity_id].draw_priority += 100;
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
};