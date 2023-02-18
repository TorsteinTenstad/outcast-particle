#pragma once
#include "components/physics.hpp"
#include "components/shader.hpp"
#include <vector>

class ScrollWindow
{
public:
	std::vector<Position*> positions;
	std::vector<Shader*> shaders;
	float scroll_position;
};