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

	Entity entity = level.GetSingletonId<SelectedEffect>([&level](ECSScene& scene) {
		return CreateScreenWideFragmentShaderEntity(level, "SHADERS_DIR\\selected_effect.frag", 50);
	});
	auto level_size = level.GetSize();
	level.GetComponent<WidthAndHeight>(entity)->width_and_height = level_size;
	level.GetComponent<Position>(entity)->position = level_size / 2.f;
	Shader* shader = level.GetComponent<Shader>(entity);
	int i = 0;
	for (const auto& [entity, selected, editable, draw_priority, width_and_height, position] : level.GetEntitiesWith<Selected, Editable, DrawPriority, WidthAndHeight, Position>())
	{
		shader->vec_uniforms["positions[" + ToString(i) + "]"] = position->position;
		shader->vec_uniforms["sizes[" + ToString(i) + "]"] = width_and_height->width_and_height;
		i++;
	}
	for (const auto& [entity, selected, editable, draw_priority, radius, position] : level.GetEntitiesWith<Selected, Editable, DrawPriority, Radius, Position>())
	{
		shader->vec_uniforms["positions[" + ToString(i) + "]"] = position->position;
		shader->vec_uniforms["sizes[" + ToString(i) + "]"] = sf::Vector2f(1, 1) * 2.f * radius->radius;
		i++;
	}

	{
		std::optional<Entity> opt_entity = level.FindSingletonId<EditModeRectangleSelectTool>();
		if (opt_entity)
		{
			Entity entity = opt_entity.value();

			auto [position, width_and_height] = level.GetComponents<Position, WidthAndHeight>(entity);
			shader->vec_uniforms["positions[" + ToString(i) + "]"] = position->position;
			shader->vec_uniforms["sizes[" + ToString(i) + "]"] = width_and_height->width_and_height;
			i++;
		}
	}
	shader->int_uniforms["n"] = i;
}