#include "SFML/Graphics/Shader.hpp"
#include "SFML/System/Vector2.hpp"
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "components/screen_wide_shader_effects.hpp"
#include "components/size.hpp"
#include "entity_creation.hpp"
#include "level.hpp"
#include "systems/_pure_DO_systems.hpp"

void ScreenWideShaderEffectsSystem::Update(Level& level, float dt)
{
	for (const auto& [entity_id, segmented_glow_effect, children, draw_priority, position] : level.GetEntitiesWith<SegmentedGlowEffect, Children, DrawPriority, Position>())
	{
		Shader* shader = EnsureExistenceOfScreenWideFragmentShaderChildEntity<SegmentedGlowEffect>(level, children, "shaders\\segmented_glow_effect.frag", draw_priority->draw_priority - 1);
		shader->vec_uniforms["origin"] = position->position;
		shader->float_uniforms["animation_start_time"] = segmented_glow_effect->animation_start_time.value_or(-1);
	}

	for (const auto& [entity_id, twinkle_effect, children, draw_priority, width_and_height, position] : level.GetEntitiesWith<TwinkleEffect, Children, DrawPriority, WidthAndHeight, Position>())
	{
		Shader* shader = EnsureExistenceOfScreenWideFragmentShaderChildEntity<TwinkleEffect>(level, children, "shaders\\twinkle.frag", draw_priority->draw_priority + 1);
		shader->vec_uniforms["position"] = position->position;
		shader->vec_uniforms["width_and_height"] = width_and_height->width_and_height;
	}
}