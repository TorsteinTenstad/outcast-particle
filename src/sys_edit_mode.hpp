#pragma once
#include "comp_draggable.hpp"
#include "comp_physics.hpp"
#include "comp_radius.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"
#include "utilityfunctions.hpp"

class EditModeSystem
{
private:
	float default_velocity_magnitude_change_sensitivity_ = 20;
	float default_velocity_angle_change_sensitivity_ = PI / 90;

public:
	void Update(CursorAndKeys& cursor_and_keys, std::map<int, Draggable>& draggable_map, std::map<int, Radius>& radius_map, std::map<int, Position>& position_map, std::map<int, Velocity>& velocity_map)
	{
		for (auto& [entity_id, draggable_entity] : draggable_map)
		{
			if (draggable_entity.being_dragged)
			{
				if (cursor_and_keys.mouse_button_released_this_frame[sf::Mouse::Left])
				{
					draggable_entity.being_dragged = false;
				}
				else
				{
					position_map[entity_id].position = cursor_and_keys.cursor_position - draggable_entity.offset;

					float velocity_magnitude = Magnitude(velocity_map[entity_id].velocity);
					float velocity_angle = Angle(velocity_map[entity_id].velocity);

					float velocity_magnitude_change_sensitivity = default_velocity_magnitude_change_sensitivity_;
					float velocity_angle_change_sensitivity = default_velocity_angle_change_sensitivity_;
					if (cursor_and_keys.key_down[sf::Keyboard::LControl])
					{
						velocity_magnitude_change_sensitivity = default_velocity_magnitude_change_sensitivity_ / 4;
						velocity_angle_change_sensitivity = default_velocity_angle_change_sensitivity_ / 4;
					}

					if (cursor_and_keys.key_down[sf::Keyboard::W])
					{
						velocity_magnitude += velocity_magnitude_change_sensitivity;
					}
					if (cursor_and_keys.key_down[sf::Keyboard::S])
					{
						velocity_magnitude -= velocity_magnitude_change_sensitivity;
					}
					if (cursor_and_keys.key_down[sf::Keyboard::D])
					{
						velocity_angle += velocity_angle_change_sensitivity;
					}
					if (cursor_and_keys.key_down[sf::Keyboard::A])
					{
						velocity_angle -= velocity_angle_change_sensitivity;
					}

					velocity_map[entity_id].velocity.x = velocity_magnitude * std::cos(velocity_angle);
					velocity_map[entity_id].velocity.y = velocity_magnitude * std::sin(velocity_angle);
				}
			}
			else if (cursor_and_keys.mouse_button_pressed_this_frame[sf::Mouse::Left] && Magnitude(cursor_and_keys.cursor_position - position_map[entity_id].position) < radius_map[entity_id].radius)
			{
				draggable_entity.being_dragged = true;
				draggable_entity.offset = cursor_and_keys.cursor_position - position_map[entity_id].position;
			}
		}
	}
};