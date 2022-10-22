#include "PCH.hpp"
#include "draw_info.hpp"
#include "position.hpp"

class RenderSystem
{
private:
	std::map<std::string, sf::Texture> textures_;

public:
	void Update(sf::RenderWindow& window, std::map<int, DrawInfo> drawinfo);
};