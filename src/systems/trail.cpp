#include "_pure_DO_systems.hpp"
#include "entity_creation.hpp"
#include "level.hpp"
#include "utils/math.hpp"
#include "utils/string_parsing.hpp"
#include <math.h>

void TrailSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, trail, children, radius, draw_priority, position] : level.GetEntitiesWith<Trail, Children, Radius, DrawPriority, Position>())
	{
		Shader* shader = EnsureExistanceOfScreenwideFragmentShaderChildEntity<Trail>(level, children, "shaders\\trail.frag", draw_priority->draw_priority - 1);

		trail->seconds_until_next_update -= dt;
		if (trail->seconds_until_next_update > 0)
		{
			return;
		}
		trail->seconds_until_next_update += trail->trail_length_in_seconds / trail->max_segments;
		trail->segments_created = std::min(trail->segments_created + 1, trail->max_segments);

		int& i = trail->segment_to_update_next;
		int n = trail->max_segments;
		shader->int_uniforms["segment_last_updated"] = i;
		shader->int_uniforms["segments_created"] = trail->segments_created;
		shader->vec_uniforms["path[" + ToString(i) + "]"] = position->position;
		shader->float_uniforms["radius"] = radius->radius;
		i = (i + 1) % n;
	}
}