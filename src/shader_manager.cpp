#include "shader_manager.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <string>

static std::optional<std::string> ReadFromFileWithRecursiveInclude(const std::string& path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::cerr << "Error: could not open file \"" << path << "\"\n";
		return std::nullopt;
	}

	std::string contents((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	std::regex include_regex("#include\\s+\"([^\"]+)\"\\s*;?");
	std::smatch include_match;

	while (std::regex_search(contents, include_match, include_regex))
	{
		std::string include_path = include_match[1].str();
		std::optional<std::string> included_contents = ReadFromFileWithRecursiveInclude(include_path);

		if (!included_contents) { continue; }

		contents.replace(include_match.position(), include_match.length(), *included_contents);
	}

	return contents;
}

static void LoadShader(sf::Shader& new_sfml_shader, const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	std::optional<std::string> vertex_shader = ReadFromFileWithRecursiveInclude(vertex_shader_path);
	std::optional<std::string> fragment_shader = ReadFromFileWithRecursiveInclude(fragment_shader_path);

	if (vertex_shader && fragment_shader)
	{
		new_sfml_shader.loadFromMemory(vertex_shader.value(), fragment_shader.value());
	}
	else if (vertex_shader)
	{
		new_sfml_shader.loadFromMemory(vertex_shader.value(), sf::Shader::Vertex);
	}
	else if (fragment_shader)
	{
		new_sfml_shader.loadFromMemory(fragment_shader.value(), sf::Shader::Fragment);
	}
	else
	{
		assert(false);
	}
}

sf::Shader& ShaderManager::GetLoadedSFMLShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	std::tuple<std::string, std::string> shader_id = { vertex_shader_path, fragment_shader_path };
	auto it = shaders_.find(shader_id);
	if (it != shaders_.end())
	{
		return it->second;
	}
	else
	{
		sf::Shader& new_sfml_shader = shaders_[shader_id];
		LoadShader(new_sfml_shader, vertex_shader_path, fragment_shader_path);
		return new_sfml_shader;
	}
}

void ShaderManager::Clear()
{
	shaders_.clear();
}
