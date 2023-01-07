#pragma once
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Shader.hpp"
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
			shader->vec_uniforms["_origin"] = (sf::Vector2f)globals.render_window.mapCoordsToPixel(position->position);
			shader->vec_uniforms["_window_resolution"] = sf::Vector2f(globals.render_window.getSize());
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};