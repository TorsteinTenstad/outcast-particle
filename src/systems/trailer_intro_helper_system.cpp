#pragma once
#include "components/draw_info.hpp"
#include "components/position.hpp"
#include "systems/_pure_DO_systems.hpp"

void TrailerIntroHelperSystem::Update(Level& level, float dt)
{
	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Num1])
	{
		Entity player = level.AddBlueprint(BPPlayer);
		level.GetComponent<Position>(player)->position = level.GetSize() / 2.f;
	}
}