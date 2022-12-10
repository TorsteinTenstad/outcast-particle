#pragma once
#include "PCH.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "trail.hpp"
#include "utils.hpp"

class RenderTrailSystem : public GameSystem
{
private:
	std::map<int, std::array<sf::ConvexShape, TRAIL_N>> segments_;

public:
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		auto& draw_priority_map = level.GetComponent<DrawPriority>();
		auto& position_map = level.GetComponent<Position>();
		auto& trail_map = level.GetComponent<Trail>();

		for (auto [entity_id, trail] : trail_map)
		{
			assert(draw_priority_map.count(entity_id) > 0);
			sf::Vector2f global_segment_position = position_map[entity_id].position;
			sf::Vector2f p1 = global_segment_position;
			sf::Vector2f p2 = global_segment_position;
			sf::Vector2f p3;
			sf::Vector2f p4;
			sf::Vector2f unit_normal = GetQuarterTurnRotation(Normalized(trail_map[entity_id].path[0]));
			p1 -= trail_map[entity_id].widths[0] * unit_normal;
			p2 += trail_map[entity_id].widths[0] * unit_normal;
			for (unsigned i = 0; i < (trail_map[entity_id].path.size() - 1); ++i)
			{
				unit_normal = GetQuarterTurnRotation(Normalized(trail_map[entity_id].path[i + 1]));
				p3 = global_segment_position + trail_map[entity_id].path[i] + trail_map[entity_id].widths[i + 1] * unit_normal;
				p4 = global_segment_position + trail_map[entity_id].path[i] - trail_map[entity_id].widths[i + 1] * unit_normal;
				segments_[entity_id][i] = sf::ConvexShape();
				segments_[entity_id][i].setPointCount(4);
				segments_[entity_id][i].setPoint(0, p1);
				segments_[entity_id][i].setPoint(1, p2);
				segments_[entity_id][i].setPoint(2, p3);
				segments_[entity_id][i].setPoint(3, p4);
				segments_[entity_id][i].setFillColor(sf::Color(200, 200, 200, 127 * pow((float)(trail_map[entity_id].path.size() - i) / trail_map[entity_id].path.size(), 2)));
				level.drawables[draw_priority_map[entity_id].draw_priority].push_back({ entity_id, &segments_[entity_id][i] });
				global_segment_position += trail_map[entity_id].path[i];
				p1 = p4;
				p2 = p3;
			}
		}
	}
};