#pragma once
#include "PCH.hpp"

class Shader
{
public:
	std::string vertex_shader_path;
	std::string fragment_shader_path;
	std::map<std::string, float> uniforms;
	sf::Shader* shader = nullptr;
};