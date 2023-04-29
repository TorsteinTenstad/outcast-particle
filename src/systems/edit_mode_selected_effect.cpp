#include "SFML/Graphics/Shader.hpp"
#include "SFML/System/Vector2.hpp"
#include "components/draw_info.hpp"
#include "components/editable.hpp"
#include "components/position.hpp"
#include "components/size.hpp"
#include "entity_creation.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include "utils/string_parsing.hpp"
#include <functional>

class SelectedEffect
{};

void EditModeSelectedEffectSystem::Update(Level& level, float dt)
{
	if (level.GetMode() != EDIT_MODE)
	{
		level.DeleteEntitiesWith<SelectedEffect>();
		return;
	}

	int entity_id = level.GetSingletonId<SelectedEffect>([&level](ECSScene& scene) {
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
		std::optional<int> opt_entity_id = level.FindSingletonId<EditModeRectangleSelectTool>();
		if (opt_entity_id)
		{
			int entity_id = opt_entity_id.value();

			auto [position, width_and_height] = level.GetComponents<Position, WidthAndHeight>(entity_id);
			shader->vec_uniforms["positions[" + ToString(i) + "]"] = position->position;
			shader->vec_uniforms["sizes[" + ToString(i) + "]"] = width_and_height->width_and_height;
			i++;
		}
	}
	shader->int_uniforms["n"] = i;
}