#pragma once
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <string>

class ShaderManager
{
private:
	std::map<std::tuple<std::string, std::string>, sf::Shader> shaders_;

public:
	sf::Shader& GetLoadedSFMLShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
    void Clear();
};