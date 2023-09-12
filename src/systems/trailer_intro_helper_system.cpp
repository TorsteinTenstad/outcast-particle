#pragma once
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "components/position.hpp"
#include "systems/_pure_DO_systems.hpp"

void TrailerIntroHelperSystem::Update(Level& level, float dt)
{
	sf::Vector2f level_size = level.GetSize();
	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Num1])
	{
		Entity entity = level.AddBlueprint(BPPlayer);
		level.GetComponent<Charge>(entity)->charge *= -1;
		level.GetComponent<Position>(entity)->position = sf::Vector2f(1 * level_size.x / 3, level_size.y / 2);
	}

	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Num2])
	{
		Entity entity = level.AddBlueprint(BPStaticParticle);
		level.GetComponent<Shader>(entity)->vertex_shader_path = "SHADERS_DIR\\particle.vert";
		level.GetComponent<Position>(entity)->position = sf::Vector2f(2 * level_size.x / 3, level_size.y / 2);
	}
}