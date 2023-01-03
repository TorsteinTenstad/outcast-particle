#pragma once
#include "PCH.hpp"
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
		for (const auto& [entity_id, player, force_visualization, children, radius, player_charge, player_position] : level.GetEntitiesWith<Player, ForceVisualization, Children, Radius, Charge, Position>())
		{
			if (children->ids_owned_by_component.count(typeid(ForceVisualization)) == 0)
			{
				int id = level.CreateEntityId();
				children->ids_owned_by_component[typeid(ForceVisualization)].push_back(id);
				level.GetComponent<Position>()[id].position = level.size / 2.f;
				level.GetComponent<WidthAndHeight>()[id].width_and_height = level.size;
				level.GetComponent<DrawPriority>()[id].draw_priority = 5;
				level.GetComponent<DrawInfo>()[id].image_path = "content\\textures\\transparent.png";
				level.GetComponent<Shader>()[id].fragment_shader_path = "shaders\\force.frag";
			}
			std::vector<int> visualization_entities = children->ids_owned_by_component[typeid(ForceVisualization)];
			assert(visualization_entities.size() == 1);
			int visualization_entity = visualization_entities[0];
			assert(level.GetComponent<Shader>().count(visualization_entity) > 0);
			Shader* shader = &level.GetComponent<Shader>()[visualization_entity];

			sf::Vector2i player_screen_space_position = globals.render_window.mapCoordsToPixel(player_position->position);
			shader->vec_uniforms["player_pos"] = (sf::Vector2f)player_screen_space_position;
			shader->float_uniforms["charge_radius"] = radius->radius * std::min(globals.render_window.getSize().x / level.size.x, globals.render_window.getSize().y / level.size.y);
			shader->vec_uniforms["window_resolution"] = sf::Vector2f(globals.render_window.getSize());
			shader->float_uniforms["_time"];

			int charge_i = 0;
			for (const auto [particle_entity_id, particle_charge, particle_position] : level.GetEntitiesWith<Charge, Position>())
			{
				if (particle_entity_id == entity_id)
				{
					continue;
				}
				sf::Vector2i particle_screen_space_position = globals.render_window.mapCoordsToPixel(particle_position->position);
				if (particle_screen_space_position == player_screen_space_position)
				{
					continue;
				}
				float sign = Sign(player_charge->charge * particle_charge->charge);
				float charge_force = sign * Magnitude(CalculateElectricForce(*player_position, *particle_position, *player_charge, *particle_charge));
				shader->vec_uniforms["charge_positions[" + ToString(charge_i) + "]"] = (sf::Vector2f)particle_screen_space_position;
				shader->float_uniforms["charge_force[" + ToString(charge_i) + "]"] = charge_force;
				charge_i++;
			}

			shader->int_uniforms["n_charges"] = charge_i;
		}
	}

	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};