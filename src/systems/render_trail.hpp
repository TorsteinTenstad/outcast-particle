#pragma once
#include "SFML/Graphics/ConvexShape.hpp"
#include "components/draw_info.hpp"
#include "components/physics.hpp"
#include "game_system.hpp"
#include "globals.hpp"
#include "level.hpp"
#include "trail.hpp"
#include "utils.hpp"

class RenderTrailSystem : public GameSystem
{
private:
	std::map<int, std::array<sf::ConvexShape, TRAIL_N>> segments_;

public:
	using GameSystem::GameSystem;
	void Update(Level& level, float dt);
};