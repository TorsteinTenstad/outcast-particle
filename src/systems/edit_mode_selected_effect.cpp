#include "SFML/Graphics/Shader.hpp"
#include "SFML/System/Vector2.hpp"
#include "_pure_DO_systems.hpp"
#include "entity_creation.hpp"
#include "int_tag_enum.hpp"
#include "level.hpp"
#include "utils/math.hpp"
#include "utils/string_parsing.hpp"
#include <functional>

void EditModeSelectedEffectSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != EDIT_MODE)
	{
		level.DeleteEntitiesWith<SelectedEffect>();
		return;
	}

	auto [entity_id, selected_effect] = level.GetSingletonIncludeID<SelectedEffect>([&level](ECSScene& scene) {
		return CreateScreenWideFragmentShaderEntity(level, "shaders\\selected_effect.frag", 50);
	});
	Shader* shader = level.GetComponent<Shader>(entity_id);
	int i = 0;
	for (const auto& [entity_id, selected, editable, draw_priority, width_and_height, position] : level.GetEntitiesWith<Selected, Editable, DrawPriority, WidthAndHeight, Position>())
	{
		shader->vec_uniforms["positions[" + ToString(i) + "]"] = position->position;
		shader->vec_uniforms["sizes[" + ToString(i) + "]"] = width_and_height->width_and_height;
		i++;
	}
	for (const auto& [entity_id, selected, editable, draw_priority, radius, position] : level.GetEntitiesWith<Selected, Editable, DrawPriority, Radius, Position>())
	{
		shader->vec_uniforms["positions[" + ToString(i) + "]"] = position->position;
		shader->vec_uniforms["sizes[" + ToString(i) + "]"] = sf::Vector2f(1, 1) * 2.f * radius->radius;
		i++;
	}

	{
		int entity_id = level.GetSingleton(EditModeRectangleSelectTool);
		shader->vec_uniforms["positions[" + ToString(i) + "]"] = level.GetComponent<Position>(entity_id)->position;
		shader->vec_uniforms["sizes[" + ToString(i) + "]"] = level.GetComponent<WidthAndHeight>(entity_id)->width_and_height;
		i++;
	}
	shader->int_uniforms["n"] = i;
}