#include "SFML/Graphics/Shader.hpp"
#include "SFML/System/Vector2.hpp"
#include "_pure_DO_systems.hpp"
#include "level.hpp"

void SegmentedGlowEffectSystem::Update(Level& level, float dt)
{
	for (const auto& [entity_id, segmented_glow_effect, draw_priority, position] : level.GetEntitiesWith<SegmentedGlowEffect, DrawPriority, Position>())
	{
		Shader* shader = EnsureExistanceOfScreenwideFragmentShaderChildEntity<SegmentedGlowEffect>(level, &level.GetComponent<Children>()[entity_id], "shaders\\segmented_glow_effect.frag", draw_priority->draw_priority - 1);
		shader->vec_uniforms["origin"] = position->position;
	}
}