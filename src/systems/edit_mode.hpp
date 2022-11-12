#pragma once
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "utilityfunctions.hpp"

class EditModeSystem : public GameSystem
{
private:
	float default_velocity_magnitude_change_sensitivity_ = 20;
	float default_velocity_angle_change_sensitivity_ = PI / 90;

public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		(void)dt;
		std::map<int, Position>& position_map = level.GetComponent<Position>();
		std::map<int, Velocity>& velocity_map = level.GetComponent<Velocity>();
		std::map<int, Editable>& editable_map = level.GetComponent<Editable>();
		std::map<int, ClickedOn>& clicked_on_map = level.GetComponent<ClickedOn>();
		std::map<int, WidthAndHeight>& width_and_height_map = level.GetComponent<WidthAndHeight>();
		std::map<int, Radius>& radius_map = level.GetComponent<Radius>();
		std::map<int, DrawInfo>& draw_info_map = level.GetComponent<DrawInfo>();
		std::map<int, Border>& border_map = level.GetComponent<Border>();
		std::map<int, Charge>& charge_map = level.GetComponent<Charge>();

		// Copy all selected entities on mouse press while COPY_ENTITY_KEY is down:
		for (auto& [entity_id, editable_entity] : editable_map)
		{
			if (editable_entity.selected && cursor_and_keys.key_down[COPY_ENTITY_KEY] && cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left])
			{
				int id = level.CopyEntity(entity_id);
				position_map[id].position = cursor_and_keys.cursor_position - editable_entity.drag_offset;
				editable_map[id].selected = false;
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
		for (auto& [entity_id, editable_entity] : editable_map)
		{
			// Delete entities:
			if (editable_entity.selected && cursor_and_keys.key_pressed_this_frame[DELETE_ENTITY_KEY])
			{
				level.DeleteEntity(entity_id);
			}

			// Move all selected entities with the curser when holding down left mousebutton:
			if (editable_entity.selected && cursor_and_keys.mouse_button_down[sf::Mouse::Left] && !cursor_and_keys.key_down[COPY_ENTITY_KEY])
			{
				position_map[entity_id].position = cursor_and_keys.cursor_position - editable_entity.drag_offset;
				if (cursor_and_keys.key_down[sf::Keyboard::LShift])
				{
					if (width_and_height_map.count(entity_id))
					{
						position_map[entity_id].position.x -= width_and_height_map[entity_id].width_and_height.x / 2;
						position_map[entity_id].position.y -= width_and_height_map[entity_id].width_and_height.y / 2;
						position_map[entity_id].position.x -= std::fmod(position_map[entity_id].position.x, 48);
						position_map[entity_id].position.y -= std::fmod(position_map[entity_id].position.y, 48);
						position_map[entity_id].position.x += width_and_height_map[entity_id].width_and_height.x / 2;
						position_map[entity_id].position.y += width_and_height_map[entity_id].width_and_height.y / 2;
					}
					if (radius_map.count(entity_id))
					{
						position_map[entity_id].position.x -= radius_map[entity_id].radius;
						position_map[entity_id].position.y -= radius_map[entity_id].radius;
						position_map[entity_id].position.x -= std::fmod(position_map[entity_id].position.x, 48);
						position_map[entity_id].position.y -= std::fmod(position_map[entity_id].position.y, 48);
						position_map[entity_id].position.x += radius_map[entity_id].radius;
						position_map[entity_id].position.y += radius_map[entity_id].radius;
					}
				}
			}

			if (editable_entity.selected && editable_entity.is_charge_editable && cursor_and_keys.key_pressed_this_frame[EDIT_MODE_SWITCH_CHARGE_KEY])
			{
				charge_map[entity_id].charge = -charge_map[entity_id].charge;
			}

			// Edit velocity of all selected entites with editable velocity:
			if (editable_entity.selected && editable_entity.is_velocity_editable)
			{
				float velocity_magnitude = Magnitude(velocity_map[entity_id].velocity);
				float velocity_angle = Angle(velocity_map[entity_id].velocity);

				float velocity_magnitude_change_sensitivity = default_velocity_magnitude_change_sensitivity_;
				float velocity_angle_change_sensitivity = default_velocity_angle_change_sensitivity_;
				if (cursor_and_keys.key_down[DECREASE_SENSITIVITY_KEY])
				{
					velocity_magnitude_change_sensitivity = default_velocity_magnitude_change_sensitivity_ / 4;
					velocity_angle_change_sensitivity = default_velocity_angle_change_sensitivity_ / 4;
				}

				if (cursor_and_keys.key_down[INCREMENT_VELOCITY_KEY])
				{
					velocity_magnitude += velocity_magnitude_change_sensitivity;
				}
				if (cursor_and_keys.key_down[DECREMENT_VELOCITY_KEY])
				{
					velocity_magnitude -= velocity_magnitude_change_sensitivity;
				}
				if (cursor_and_keys.key_down[INCREMENT_VELOCITY_ANGLE_KEY])
				{
					velocity_angle += velocity_angle_change_sensitivity;
				}
				if (cursor_and_keys.key_down[DECREMENT_VELOCITY_ANGLE_KEY])
				{
					velocity_angle -= velocity_angle_change_sensitivity;
				}

				velocity_map[entity_id].velocity.x = velocity_magnitude * std::cos(velocity_angle);
				velocity_map[entity_id].velocity.y = velocity_magnitude * std::sin(velocity_angle);
			}

			// Edit width, height and rotation of all selected entites with editable width and height:
			if (editable_entity.selected && editable_entity.is_height_and_widht_editable)
			{
				if (cursor_and_keys.key_pressed_this_frame[ROTATE_ENTITY_KEY])
				{
					width_and_height_map[entity_id].width_and_height = sf::Vector2f(width_and_height_map[entity_id].width_and_height.y, width_and_height_map[entity_id].width_and_height.x);
					if (draw_info_map.count(entity_id))
					{
						draw_info_map[entity_id].rotated = draw_info_map[entity_id].rotated;
					}
				}
				if (cursor_and_keys.key_pressed_this_frame[INCREMENT_HEIGHT_KEY])
				{
					width_and_height_map[entity_id].width_and_height.y += 48;
				}
				if (cursor_and_keys.key_pressed_this_frame[DECREMENT_HEIGHT_KEY])
				{
					width_and_height_map[entity_id].width_and_height.y -= 48;
					if (width_and_height_map[entity_id].width_and_height.y < 48)
					{
						width_and_height_map[entity_id].width_and_height.y = 48;
					}
				}
				if (cursor_and_keys.key_pressed_this_frame[INCREMENT_WIDTH_KEY])
				{
					width_and_height_map[entity_id].width_and_height.x += 48;
				}
				if (cursor_and_keys.key_pressed_this_frame[DECREMENT_WIDTH_KEY])
				{
					width_and_height_map[entity_id].width_and_height.x -= 48;
					if (width_and_height_map[entity_id].width_and_height.x < 48)
					{
						width_and_height_map[entity_id].width_and_height.x = 48;
					}
				}
			}
		}
	}
};