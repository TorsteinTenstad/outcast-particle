#include "components/blueprint_menu_item.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "components/screen_wide_shader_effects.hpp"
#include "components/size.hpp"
#include "entity_creation.hpp"
#include "make_fade_into_level.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "utils/math.hpp"
#include "utils/physics.hpp"
#include "utils/string_parsing.hpp"

void ForceVisualizationSystem::Update(Level& level, float dt)
{
	if (!globals.general_config.forces_are_visualized)
	{
		return;
	}
	for (const auto& [entity, force_visualization, children, radius, player_charge, draw_priority, player_position] : level.GetEntitiesWith<ForceVisualization, Children, Radius, Charge, DrawPriority, Position>())
	{
		std::function<Entity(Level&)> child_creation_func = [active_level_id = active_level_id_, draw_priority = draw_priority](Level& level) {
			Entity entity = CreateScreenWideFragmentShaderEntity(level, (SHADERS_DIR / "force.frag").string(), 9);
			MakeFadeIntoLevel(level, entity, active_level_id);
			return entity;
		};
		Entity child = GetSingletonChildId<ForceVisualization>(level, entity, child_creation_func);
		Shader* shader = level.GetComponent<Shader>(child);

		shader->vec_uniforms["player_pos"] = player_position->position;
		shader->float_uniforms["charge_radius"] = radius->radius;

		int charge_i = 0;
		for (const auto [particle_entity, particle_charge, particle_position] : level.GetEntitiesWith<Charge, Position>())
		{

			if (particle_entity == entity) { continue; }
			if (level.HasComponents<BlueprintMenuItem>(particle_entity)) { continue; }
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
