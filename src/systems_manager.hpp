#include "PCH.hpp"
#include "draw_info.hpp"
#include "render_system.hpp"
#include "velocity_system.hpp"
#include <map>

class SystemsManager
{
private:
	std::map<int, DrawInfo> draw_info_;
	std::map<int, Position> position_;
	std::map<int, Velocity> velocity_;
	RenderSystem render_system_;
	VelocitySystem velocity_system_;

public:
	SystemsManager();
	void Update(sf::RenderWindow& window, float dt);
	void AddEntity();
};