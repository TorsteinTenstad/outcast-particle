#include "_pure_DO_systems.hpp"
#include "entity_creation.hpp"
#include "level.hpp"
#include "make_fade_into_level.hpp"
#include "utils/math.hpp"
#include "utils/string_parsing.hpp"
#include <math.h>

void TrailSystem::Update(Level& level, float dt)
{
	for (auto [entity_id, trail, children, radius, draw_priority, position] : level.GetEntitiesWith<Trail, Children, Radius, DrawPriority, Position>())
	{
		std::function<int(void)> child_creation_func = [&level, active_level_id = active_level_id_, draw_priority = draw_priority]() {
			int entity_id = CreateScreenwideFragmentShaderEntity(level, "shaders\\trail.frag", draw_priority->draw_priority - 1);
			MakeFadeIntoLevel(level, entity_id, active_level_id);
			return entity_id;
		};
		int child_id = EnsureExistanceOfChildEntity<Trail>(children, child_creation_func);
		Shader* shader = level.GetComponent<Shader>(child_id);

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