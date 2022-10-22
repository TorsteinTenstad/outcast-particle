#include "PCH.hpp"
#include <map>

class SystemsManager
{
private:
	std::map<int, sf::RectangleShape> sprites_;

public:
	SystemsManager();
	void Update(sf::RenderWindow& window, float dt);
};