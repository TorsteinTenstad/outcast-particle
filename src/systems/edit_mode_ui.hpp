#pragma once

#include "SFML/Window/Mouse.hpp"
#include "game_system.hpp"
#include "level.hpp"

const std::array<std::string, 7> EDIT_TOOLS = {
	"BPDeleteIndicator",
	"BPWall",
	"BPBounceWall",
	"BPNoBounceWall",
	"BPLaser",
	"BPElectricField",
	"BPMagneticField"
};
const std::array<std::string, 7> EDIT_TOOLS_ICONS = {
	"content\\textures\\delete_indicator.png",
	"content\\textures\\block.png",
	"content\\textures\\block_green.png",
	"content\\textures\\block_dark.png",
	"content\\textures\\laser_horisontal.png",
	"content\\textures\\electric_field.png",
	"content\\textures\\magnetic_field.png"
};

class EditModeUISystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		if (level.GetMode() != EDIT_MODE)
		{
			return;
		}
		EditModeUI* ui = GetSingleton<EditModeUI>(level);
		if (!ui->initialized)
		{
			ui->initialized = true;
			{
				auto [entity_id, draw_info, fill_color, draw_priority, width_and_height, position] = level.CreateEntitiyWith<DrawInfo, FillColor, DrawPriority, WidthAndHeight, Position>();
				width_and_height->width_and_height = sf::Vector2f(EDIT_MODE_PANEL_WIDTH, level.GetSize().y);
				fill_color->color = sf::Color(50, 50, 50);
				position->position = sf::Vector2f(-float(EDIT_MODE_PANEL_WIDTH) / 2, level.GetSize().y / 2);
			}
			for (int i = 0; i < 7; ++i)
			{
				auto [entity_id, position] = level.AddBlueprintGetComponents<Position>("BPButton");
				ui->blueprint_buttons.push_back(entity_id);
				level.AddComponent<EditToolButton>(entity_id)->tool = EDIT_TOOLS[i];
				position->position = sf::Vector2f(-float(EDIT_MODE_PANEL_WIDTH) / 2, BLOCK_SIZE * (2 + 2 * i));
				auto [icon_id, draw_info, draw_priority, icon_position] = level.AddBlueprintGetComponents<DrawInfo, DrawPriority, Position>("BPTexturedRectangle");
				draw_info->image_path = EDIT_TOOLS_ICONS[i];
				draw_priority->draw_priority = UI_BASE_DRAW_PRIORITY + 1;
				icon_position->position = sf::Vector2f(-float(EDIT_MODE_PANEL_WIDTH - BLOCK_SIZE) / 2, BLOCK_SIZE * (2 + 2 * i));
			}
		}
		for (auto const& [entity_id, edit_tool_button] : level.GetEntitiesWith<EditToolButton>())
		{
			if (level.HasComponents<Hovered>(entity_id))
			{
				for (int button = 0; button < sf::Mouse::Button::ButtonCount; ++button)
				{
					if (cursor_and_keys_.mouse_button_pressed_this_frame[button])
					{
						for (auto const& [entity_id, button_bound_edit_tool] : level.GetEntitiesWith<ButtonBoundEditTool>())
						{
							if (button_bound_edit_tool->button == (sf::Mouse::Button)button)
							{
								level.RemoveComponents<ButtonBoundEditTool>(entity_id);
							}
						}
						ButtonBoundEditTool* button_bound_edit_tool = level.AddComponent<ButtonBoundEditTool>(entity_id);
						button_bound_edit_tool->button = (sf::Mouse::Button)button;
						button_bound_edit_tool->tool = edit_tool_button->tool;
					}
				}
			}
		}
	}
};