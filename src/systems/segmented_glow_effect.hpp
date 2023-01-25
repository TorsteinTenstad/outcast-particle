#pragma once
#include "SFML/Graphics/Shader.hpp"
#include "SFML/System/Vector2.hpp"
#include "game_system.hpp"
#include "level.hpp"

class SegmentedGlowEffectSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (const auto& [entity_id, segmented_glow_effect, draw_priority, position] : level.GetEntitiesWith<SegmentedGlowEffect, DrawPriority, Position>())
		{
			Shader* shader = EnsureExistanceOfScreenwideFragmentShaderChildEntity<SegmentedGlowEffect>(level, &level.GetComponent<Children>()[entity_id], "shaders\\segmented_glow_effect.frag", draw_priority->draw_priority - 1);
			shader->vec_uniforms["origin"] = position->position;
		}
	}
};