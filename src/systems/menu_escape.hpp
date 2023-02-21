#include "game_system.hpp"
#include "level.hpp"
#include <functional>

class MenuEscapeSystem : public GameSystem
{
private:
	std::function<void()> go_to_last_menu_;

public:
	using GameSystem::GameSystem;
	void Give(std::function<void()> go_to_last_menu);
	void Update(Level& level, float dt);
};