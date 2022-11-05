#pragma once
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "utilityfunctions.hpp"

static float RoundToNearest(float x, float y)
{
	float rest = std::fmod(x, y);
	if (rest < y / 2)
	{
		return x - rest;
	}
	else
	{
		return x + y - rest;
	}
}

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
		std::map<int, Draggable>& draggable_map = level.GetComponent<Draggable>();
		std::map<int, ClickedOn>& clicked_on_map = level.GetComponent<ClickedOn>();

		for (auto& [entity_id, draggable_entity] : draggable_map)
		{
			if (draggable_entity.being_dragged)
			{
				if (clicked_on_map[entity_id].released_this_frame)
				{
					draggable_entity.being_dragged = false;

					if (cursor_and_keys.key_down[sf::Keyboard::LControl])
					{
						// Make a copy of the entity and place the copy where the entity was picked up:
						int id = level.CopyEntity(entity_id);
						position_map[id].position = cursor_and_keys.mouse_button_last_pressed_position[sf::Mouse::Left] - draggable_entity.offset;
					}
				}
				else
				{
					position_map[entity_id].position = cursor_and_keys.cursor_position - draggable_entity.offset;
					if (cursor_and_keys.key_down[sf::Keyboard::LShift])
					{
						position_map[entity_id].position.x = RoundToNearest(position_map[entity_id].position.x, 48);
						position_map[entity_id].position.y = RoundToNearest(position_map[entity_id].position.y, 48);
					}

					float velocity_magnitude = Magnitude(velocity_map[entity_id].velocity);
					float velocity_angle = Angle(velocity_map[entity_id].velocity);

					float velocity_magnitude_change_sensitivity = default_velocity_magnitude_change_sensitivity_;
					float velocity_angle_change_sensitivity = default_velocity_angle_change_sensitivity_;
					if (cursor_and_keys.key_down[sf::Keyboard::LAlt])
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
			else if (clicked_on_map[entity_id].clicked_this_frame)
			{
				draggable_entity.being_dragged = true;
				draggable_entity.offset = cursor_and_keys.cursor_position - position_map[entity_id].position;
			}
		}
	}
};