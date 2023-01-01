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
		if (level.GetComponent<Player>().size() > 0 && level.GetComponent<ForceVisualization>().size() < 1)
		{
			int id = level.CreateEntityId();
			level.GetComponent<DrawPriority>()[id].draw_priority = 5;
			level.GetComponent<DrawInfo>()[id].image_path = "content\\textures\\transparent.png";
			level.GetComponent<Shader>()[id].fragment_shader_path = "shaders\\force.frag";
			level.GetComponent<ForceVisualization>()[id];
		}
		if (level.GetComponent<Player>().size() == 0 && level.GetComponent<ForceVisualization>().size() > 0)
		{
			for (auto it = level.GetComponent<ForceVisualization>().cbegin(), next_it = it; it != level.GetComponent<ForceVisualization>().cend(); it = next_it)
			{
				++next_it;
				auto entity_id = it->first;
				level.DeleteEntity(entity_id);
			}
		}
		for (const auto& [entity_id, force_visualization] : level.GetComponent<ForceVisualization>())
		{
			level.GetComponent<WidthAndHeight>()[entity_id].width_and_height = level.size;
			level.GetComponent<Position>()[entity_id].position = level.size / 2.f;
			level.GetComponent<Shader>()[entity_id].vec_uniforms["window_resolution"] = sf::Vector2f(globals.render_window.getSize());
			Position player_position;
			Charge player_charge;
			for (const auto& [player_id, player] : level.GetComponent<Player>())
			{
				player_position = level.GetComponent<Position>()[player_id];
				player_charge = level.GetComponent<Charge>()[player_id];
				sf::Vector2f player_pos = (sf::Vector2f)globals.render_window.mapCoordsToPixel(player_position.position);
				level.GetComponent<Shader>()[entity_id].vec_uniforms["player_pos"] = player_pos;
				float charge_radius = level.GetComponent<Radius>()[player_id].radius;
				charge_radius *= std::min(globals.render_window.getSize().x / level.size.x, globals.render_window.getSize().y / level.size.y);
				level.GetComponent<Shader>()[entity_id].float_uniforms["charge_radius"] = charge_radius;
			}
			int charge_i = 0;
			for (const auto [charge_entity_id, charge] : level.GetComponent<Charge>())
			{
				if (level.GetComponent<Player>().count(charge_entity_id) == 0)
				{
					Position particle_position = level.GetComponent<Position>()[charge_entity_id];
					sf::Vector2f charge_position = (sf::Vector2f)globals.render_window.mapCoordsToPixel(particle_position.position);
					float sign = Sign(player_charge.charge * level.GetComponent<Charge>()[charge_entity_id].charge);
					float charge_force = sign * Magnitude(CalculateElectricForce(player_position, particle_position, player_charge, level.GetComponent<Charge>()[charge_entity_id]));
					level.GetComponent<Shader>()[entity_id].vec_uniforms["charge_positions[" + ToString(charge_i) + "]"] = charge_position;
					level.GetComponent<Shader>()[entity_id].float_uniforms["charge_force[" + ToString(charge_i) + "]"] = charge_force;
					charge_i++;
				}
			}
			level.GetComponent<Shader>()[entity_id].int_uniforms["n_charges"] = charge_i;
			level.GetComponent<Shader>()[entity_id].float_uniforms["_time"];
		}
	}
	void OnEnterMode(Level& level) {};
	void OnExitMode(Level& level) {};
	void OnEnterLevel(Level& level) {};
	void OnExitLevel(Level& level) {};
};