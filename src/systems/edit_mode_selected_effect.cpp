#include "SFML/Graphics/Shader.hpp"
#include "SFML/System/Vector2.hpp"
#include "_pure_DO_systems.hpp"
#include "entity_creation.hpp"
#include "level.hpp"
#include "utils/string_parsing.hpp"

void EditModeSelectedEffectSystem::Update(Level& level, float dt)
{
	if (!in_edit_mode_)
	{
		return;
	}

	auto [entity_id, selected_effect] = level.GetSingletonIncludeID<SelectedEffect>([&level](ECSScene& scene) {
		return CreateScreenWideFragmentShaderEntity(level, "shaders\\selected_effect.frag", 0);
	});
	Shader* shader = level.GetComponent<Shader>(entity_id);
	int max_draw_priority = 0;
	int i = 0;
	for (const auto& [entity_id, selected, editable, draw_priority, width_and_height, position] : level.GetEntitiesWith<Selected, Editable, DrawPriority, WidthAndHeight, Position>())
	{
		max_draw_priority = std::max(max_draw_priority, draw_priority->draw_priority);
		shader->vec_uniforms["positions[" + ToString(i) + "]"] = position->position;
		shader->vec_uniforms["sizes[" + ToString(i) + "]"] = width_and_height->width_and_height;
		i++;
	}
	for (const auto& [entity_id, selected, editable, draw_priority, radius, position] : level.GetEntitiesWith<Selected, Editable, DrawPriority, Radius, Position>())

	{
		max_draw_priority = std::max(max_draw_priority, draw_priority->draw_priority);
		shader->vec_uniforms["positions[" + ToString(i) + "]"] = position->position;
		shader->vec_uniforms["sizes[" + ToString(i) + "]"] = sf::Vector2f(1, 1) * 2.f * radius->radius;
		i++;
	}
	shader->int_uniforms["n"] = i;
	level.GetComponent<DrawPriority>(entity_id)->draw_priority = max_draw_priority + 1;
}