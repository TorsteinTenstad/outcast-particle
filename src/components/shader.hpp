#pragma once
#include "SFML/Graphics/Glsl.hpp"
#include "SFML/System/Vector2.hpp"
#include <map>
#include <string>

class Shader
{
public:
	std::string vertex_shader_path = "";
	std::string fragment_shader_path = "";
	std::map<std::string, int> int_uniforms;
	std::map<std::string, float> float_uniforms;
	std::map<std::string, sf::Vector2f> vec_uniforms;
	std::map<std::string, sf::Glsl::Vec4> vec4_uniforms;
};