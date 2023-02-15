#pragma once
#include "components/editable.hpp"
#include "components/physics.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "utils.hpp"

static sf::Vector2f SnapToGrid(sf::Vector2f v, float grid_size)
{
	v.x -= std::fmod(v.x, grid_size);
	v.y -= std::fmod(v.y, grid_size);
	return v;
}

class EditModeSystem : public GameSystem
{
private:
	using GameSystem::GameSystem;
	const float default_velocity_magnitude_change_sensitivity_ = 400;
	const float default_velocity_angle_change_sensitivity_ = PI / 2;
	const std::vector<std::string> blueprint_menu_entry_tags_ { "BPStaticParticle", "BPPlayer", "BPLaser", "BPWall", "BPBounceWall", "BPNoBounceWall", "BPGoal", "BPElectricField", "BPMagneticField", "BPCoin", "BPBlackHole" };

	bool blueprint_menu_is_open = false;

public:
	void Update(Level& level, float dt)
	{
		return;
		if (level.GetMode() != EDIT_MODE)
		{
			return;
		}

		// Mark selected entities with border:
		level.GetComponent<FillColor>().clear();
		for (auto [entity_id, selected] : level.GetEntitiesWith<Selected>())
		{
			level.GetComponent<FillColor>()[entity_id].color = sf::Color(150, 150, 255);
		}
		for (auto [entity_id, selected] : level.GetEntitiesWith<TemporarilySelected>())
		{
			level.GetComponent<FillColor>()[entity_id].color = sf::Color(150, 150, 255);
		}

		// Delete entities:
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DELETE_ENTITY])
		{
			level.DeleteEntitiesWith<Selected>();
		}

		// Conditional deselect all:
		if (cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left] && level.GetEntitiesWith<PressedThisFrame, Selected>().size() == 0
			&& !cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY] && !cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES])
		{
			level.GetComponent<Selected>().clear();
		}

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
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.INCREASE_LEVEL_SIZE])
		{
			level.IncreaseSize();
		}
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DECREASE_LEVEL_SIZE])
		{
			level.DecreaseSize();
		}
		// Copy entities:
		for (auto [entity_id, selected] : level.GetEntitiesWith<Selected>())
		{
			if (cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY] && cursor_and_keys_.mouse_button_pressed_this_frame[sf::Mouse::Left])
			{
				int new_id = level.CopyEntity(entity_id);
				level.GetComponent<Selected>().erase(new_id);
				level.GetComponent<Position>()[new_id].position = SnapToGrid(cursor_and_keys_.cursor_position - selected->mouse_offset, BLOCK_SIZE / 2);
			}
		}

		// Select entities:
		for (auto [entity_id, editable, pressed_this_frame] : level.GetEntitiesWith<Editable, PressedThisFrame>())
		{
			level.GetComponent<Selected>()[entity_id];
			for (auto [entity_id, selected, position] : level.GetEntitiesWith<Selected, Position>())
			{
				selected->mouse_offset = cursor_and_keys_.cursor_position - position->position;
			}
		}

		// Move entities with the curser:
		if (cursor_and_keys_.mouse_button_down[sf::Mouse::Left] && !cursor_and_keys_.key_down[globals.key_config.COPY_ENTITY] && !cursor_and_keys_.key_down[globals.key_config.SELECT_MULTIPLE_ENTITIES])
		{
			for (auto [entity_id, selected, position] : level.GetEntitiesWith<Selected, Position>())
			{
				position->position = SnapToGrid(cursor_and_keys_.cursor_position - selected->mouse_offset, BLOCK_SIZE / 2);
			}
		}

		// Handle selection of entity in blueprint menu:
		for (auto& [entity_id, blueprint_menu_item, selected, draw_priority] : level.GetEntitiesWith<BlueprintMenuItem, Selected, DrawPriority>())
		{
			level.GetComponent<BlueprintMenuItem>().erase(entity_id);
			draw_priority->draw_priority -= UI_BASE_DRAW_PRIORITY;
			CloseBlueprintMenu(level);
		}

		// Edit charge:
		for (auto [entity_id, selected, charge] : level.GetEntitiesWith<Selected, Charge>())
		{
			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.EDIT_MODE_SWITCH_CHARGE])
			{
				charge->charge *= -1;
			}

			for (unsigned i = 0; i < CHARGE_CATEGORY_KEYS.size(); ++i)
			{
				if (cursor_and_keys_.key_pressed_this_frame[CHARGE_CATEGORY_KEYS[i]])
				{
					charge->charge = Sign(charge->charge) * abs(PARTICLE_CHARGE_CATEGORIES[i]);
				}
			}
		}

		// Edit velocity:
		for (auto [entity_id, selected, velocity] : level.GetEntitiesWith<Selected, Velocity>())
		{
			float velocity_magnitude = Magnitude(velocity->velocity);
			float velocity_angle = Angle(velocity->velocity);

			float sensitivity_modifier = cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1;
			if (cursor_and_keys_.key_down[globals.key_config.INCREMENT_VELOCITY])
			{
				velocity_magnitude += default_velocity_magnitude_change_sensitivity_ * dt / sensitivity_modifier;
			}
			if (cursor_and_keys_.key_down[globals.key_config.DECREMENT_VELOCITY])
			{
				velocity_magnitude -= default_velocity_magnitude_change_sensitivity_ * dt / sensitivity_modifier;
			}
			if (cursor_and_keys_.key_down[globals.key_config.INCREMENT_VELOCITY_ANGLE])
			{
				velocity_angle += default_velocity_angle_change_sensitivity_ * dt / sensitivity_modifier;
			}
			if (cursor_and_keys_.key_down[globals.key_config.DECREMENT_VELOCITY_ANGLE])
			{
				velocity_angle -= default_velocity_angle_change_sensitivity_ * dt / sensitivity_modifier;
			}
			velocity->velocity.x = velocity_magnitude * std::cos(velocity_angle);
			velocity->velocity.y = velocity_magnitude * std::sin(velocity_angle);
		}
		// Edit magnetic field:
		for (auto [entity_id, selected, magnetic_field] : level.GetEntitiesWith<Selected, MagneticField>())
		{
			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.EDIT_MODE_SWITCH_MAGNETIC_FIELD_DIRECTION])
			{
				magnetic_field->field_strength *= -1;
			}
			for (unsigned i = 0; i < MAGNETIC_FIELD_CATEGORY_KEYS.size(); ++i)
			{
				if (cursor_and_keys_.key_pressed_this_frame[MAGNETIC_FIELD_CATEGORY_KEYS[i]])
				{
					magnetic_field->field_strength = Sign(magnetic_field->field_strength) * abs(MAGNETIC_FIELD_STRENGTH_CATEGORIES[i]);
				}
			}
		}

		// Edit electric field:
		for (auto [entity_id, selected, electric_field] : level.GetEntitiesWith<Selected, ElectricField>())
		{
			for (unsigned i = 0; i < ELECTRIC_FIELD_CATEGORY_KEYS.size(); ++i)
			{
				if (cursor_and_keys_.key_pressed_this_frame[ELECTRIC_FIELD_CATEGORY_KEYS[i]])
				{
					electric_field->field_vector = Normalized(electric_field->field_vector) * abs(ELECTRIC_FIELD_STRENGTH_CATEGORIES[i]);
				}
			}
		}

		// Edit rotation:
		auto& electric_field_map = level.GetComponent<ElectricField>();
		if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.ROTATE_ENTITY])
		{
			for (auto [entity_id, selected, width_and_height] : level.GetEntitiesWith<Selected, WidthAndHeight>())
			{
				width_and_height->width_and_height = sf::Vector2f(width_and_height->width_and_height.y, width_and_height->width_and_height.x);
				if (electric_field_map.count(entity_id) > 0)
				{
					electric_field_map[entity_id].field_vector = GetQuarterTurnRotation(electric_field_map[entity_id].field_vector);
				}
			}
		}

		// Edit rectangular size:
		for (auto [entity_id, selected, width_and_height, editable] : level.GetEntitiesWith<Selected, WidthAndHeight, Editable>())
		{
			float increment = BLOCK_SIZE * (cursor_and_keys_.key_down[globals.key_config.ALT_SENSITIVITY] ? 4 : 1);
			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.INCREMENT_HEIGHT])
			{
				width_and_height->width_and_height.y += increment;
				width_and_height->width_and_height.y -= std::fmod(width_and_height->width_and_height.y, increment);
			}
			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DECREMENT_HEIGHT])
			{
				width_and_height->width_and_height.y -= increment;
				width_and_height->width_and_height.y -= std::fmod(width_and_height->width_and_height.y, increment);
				if (width_and_height->width_and_height.y < editable->smallest_allowed_size)
				{
					width_and_height->width_and_height.y = editable->smallest_allowed_size;
				}
			}
			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.INCREMENT_WIDTH])
			{
				width_and_height->width_and_height.x += increment;
				width_and_height->width_and_height.x -= std::fmod(width_and_height->width_and_height.x, increment);
			}
			if (cursor_and_keys_.key_pressed_this_frame[globals.key_config.DECREMENT_WIDTH])
			{
				width_and_height->width_and_height.x -= increment;
				width_and_height->width_and_height.x -= std::fmod(width_and_height->width_and_height.x, increment);
				if (width_and_height->width_and_height.x < editable->smallest_allowed_size)
				{
					width_and_height->width_and_height.x = editable->smallest_allowed_size;
				}
			}
		}

		//Limit position:
		for (auto [entity_id, selected, position] : level.GetEntitiesWith<Selected, Position>())
		{
			if (position->position.x < 0)
			{
				position->position.x = 0;
			}
			else if (position->position.x > level.GetSize().x)
			{
				position->position.x = level.GetSize().x;
			}
			if (position->position.y < 0)
			{
				position->position.y = 0;
			}
			else if (position->position.y > level.GetSize().y)
			{
				position->position.y = level.GetSize().y;
			}
		}
	}
	void OpenBlueprintMenu(Level& level)
	{
		int i = 0;
		int menu_background_id = level.CreateEntityId();
		level.GetComponent<Position>()[menu_background_id].position = level.GetSize() / 2.f;
		level.GetComponent<DrawInfo>()[menu_background_id].image_path = "content\\textures\\gray.png";
		level.GetComponent<DrawPriority>()[menu_background_id].draw_priority = UI_BASE_DRAW_PRIORITY;
		level.GetComponent<ReceivesButtonEvents>()[menu_background_id];
		float menu_width = (3 * blueprint_menu_entry_tags_.size() + 1) * BLOCK_SIZE;
		level.GetComponent<WidthAndHeight>()[menu_background_id].width_and_height = sf::Vector2f(menu_width, 4 * BLOCK_SIZE);
		level.GetComponent<Border>()[menu_background_id];
		level.GetComponent<BlueprintMenuItem>()[menu_background_id];
		int entity_id;
		for (const auto& tag : blueprint_menu_entry_tags_)
		{
			entity_id = level.AddBlueprint(tag);
			level.GetComponent<Position>()[entity_id].position = sf::Vector2f(level.GetSize().x / 2 - menu_width / 2 + (2 + 3 * i) * BLOCK_SIZE, level.GetSize().y / 2);
			level.GetComponent<DrawPriority>()[entity_id].draw_priority += UI_BASE_DRAW_PRIORITY;
			level.GetComponent<BlueprintMenuItem>()[entity_id];
			i++;
		}
		blueprint_menu_is_open = true;
	}
	void CloseBlueprintMenu(Level& level)
	{
		level.DeleteEntitiesWith<BlueprintMenuItem>();
		blueprint_menu_is_open = false;
	}
};