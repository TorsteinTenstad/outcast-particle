#include "_pure_DO_systems.hpp"
#include "entity_creation.hpp"
#include "make_fade_into_level.hpp"
#include "physics_utils.hpp"
#include "string_parsing_utils.hpp"
#include "utils.hpp"

void ForceVisualizationSystem::Update(Level& level, float dt)
{
	if (!globals.general_config.forces_are_visualized)
	{
		return;
	}
	for (const auto& [entity_id, force_visualization, children, radius, player_charge, draw_priority, player_position] : level.GetEntitiesWith<ForceVisualization, Children, Radius, Charge, DrawPriority, Position>())
	{
		std::function<int(void)> child_creation_func = [&level, active_level_id = active_level_id_, draw_priority = draw_priority]() {
			int entity_id = CreateScreenwideFragmentShaderEntity(level, "shaders\\force.frag", 9);
			MakeFadeIntoLevel(level, entity_id, active_level_id);
			return entity_id;
		};
		int child_id = EnsureExistanceOfChildEntity<ForceVisualization>(children, child_creation_func);
		Shader* shader = level.GetComponent<Shader>(child_id);

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
