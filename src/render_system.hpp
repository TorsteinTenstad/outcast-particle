#include "PCH.hpp"

class RenderSystem
{
private:
	std::map<std::string, sf::Texture> textures_;

public:
	void Update(std::map<int, PositionAndSize> position, std::map<int, DrawInfo> drawinfo);
}