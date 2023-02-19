#pragma once
#include "components/physics.hpp"
#include "components/shader.hpp"
#include <vector>

class ScrollWindow
{
public:
	std::vector<Shader*> shaders;
	std::vector<Position*> positions;
};