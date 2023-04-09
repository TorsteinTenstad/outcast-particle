#pragma once
#include "SFML/Graphics/ConvexShape.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "systems/render_trail.hpp"
#include "trail.hpp"
#include "utils/math.hpp"

void RenderTrailSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, trail, radius, draw_priority, position] : level.GetEntitiesWith<Trail, Radius, DrawPriority, Position>())
	{
		if (trail->path.size() == 0)
		{
			return;
		}
		sf::Vector2f global_segment_position = position->position;
		sf::Vector2f p1 = global_segment_position;
		sf::Vector2f p2 = global_segment_position;
		sf::Vector2f p3;
		sf::Vector2f p4;
		sf::Vector2f unit_normal = GetQuarterTurnRotation(Normalized(trail->path[0]));
		p1 -= radius->radius * trail->widths[0] * unit_normal;
		p2 += radius->radius * trail->widths[0] * unit_normal;
		for (unsigned i = 0; i < (trail->path.size() - 1); ++i)
		{
			unit_normal = GetQuarterTurnRotation(Normalized(trail->path[i + 1]));
			p3 = global_segment_position + trail->path[i] + radius->radius * trail->widths[i + 1] * unit_normal;
			p4 = global_segment_position + trail->path[i] - radius->radius * trail->widths[i + 1] * unit_normal;
			segments_[entity_id][i] = sf::ConvexShape();
			segments_[entity_id][i].setPointCount(4);
			segments_[entity_id][i].setPoint(0, p1);
			segments_[entity_id][i].setPoint(1, p2);
			segments_[entity_id][i].setPoint(2, p3);
			segments_[entity_id][i].setPoint(3, p4);
			segments_[entity_id][i].setFillColor(sf::Color(200, 200, 200, 127 * pow((float)(trail->path.size() - i) / trail->path.size(), 2)));
			level.drawables[draw_priority->draw_priority].push_back({ &segments_[entity_id][i] });
			global_segment_position += trail->path[i];
			p1 = p4;
			p2 = p3;
		}
	}
}