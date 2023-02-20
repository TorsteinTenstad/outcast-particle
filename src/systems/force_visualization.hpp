#pragma once

#include "force_visualization.hpp"
#include "game_system.hpp"
#include "level.hpp"
#include "string_parsing_utils.hpp"
#include "systems/electric_force.hpp"

class ForceVisualizationSystem : public GameSystem
{
public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt)
	{
		for (const auto& [entity_id, player, force_visualization, children, radius, player_charge, draw_priority, player_position] : level.GetEntitiesWith<Player, ForceVisualization, Children, Radius, Charge, DrawPriority, Position>())
		{
			Shader* shader = EnsureExistanceOfScreenwideFragmentShaderChildEntity<ForceVisualization>(level, children, "shaders\\force.frag", 5);

			shader->vec_uniforms["player_pos"] = player_position->position;
			shader->float_uniforms["charge_radius"] = radius->radius;

			int charge_i = 0;
			for (const auto [particle_entity_id, particle_charge, particle_position] : level.GetEntitiesWith<Charge, Position>())
			{
				if (particle_entity_id == entity_id)
				{
					continue;
				}
				sf::Vector2i particle_screen_space_position = globals.render_window.mapCoordsToPixel(particle_position->position);
				if (Magnitude(player_position->position - particle_position->position) < radius->radius / 4)
				{
					continue;
				}
				float sign = Sign(player_charge->charge * particle_charge->charge);
				float charge_force = sign * Magnitude(CalculateElectricForce(player_position, particle_position, player_charge, particle_charge));
				shader->vec_uniforms["charge_positions[" + ToString(charge_i) + "]"] = particle_position->position;
				shader->float_uniforms["charge_force[" + ToString(charge_i) + "]"] = charge_force;
				charge_i++;
			}

			shader->int_uniforms["n_charges"] = charge_i;
		}
	}
};