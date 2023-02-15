#pragma once

#include "game_system.hpp"
#include "level.hpp"

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
			for (int i = 0; i < 6; ++i)
			{
				auto [entity_id, position] = level.AddBlueprint<Position>("BPButton");
				ui->blueprint_buttons.push_back(entity_id);
				level.AddComponent<StickyButton>(entity_id);
				position->position = sf::Vector2f(-float(EDIT_MODE_PANEL_WIDTH) / 2, BLOCK_SIZE * (2 + 3 * i));
			}
		}
		int i = 0;
		for (int entity_id : ui->blueprint_buttons)
		{
			if (level.HasComponents<StickyButtonDown>(entity_id))
			{
				ui->active_blueprint = std::vector<std::string>({ "BPWall", "BPBounceWall", "BPNoBounceWall", "BPLaser", "BPElectricField", "BPMagneticField" })[i];
			}
			i++;
		}
	}
};