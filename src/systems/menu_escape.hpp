#include "game.hpp"
#include "utils.hpp"
#include <algorithm>

class MenuEscapeSystem : public GameSystem
{
private:
	std::function<void(std::string)> set_level_;

public:
	using GameSystem::GameSystem;
	void Give(std::function<void(std::string)> set_level)
	{
		set_level_ = set_level;
	}
	void Update(Level& level, float dt)
	{
		if (!IsMenu(active_level_id_))
		{
			return;
		}
		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape] && (active_level_id_ == LEVEL_MENU || active_level_id_ == OPTIONS_MENU))
		{
			set_level_(MAIN_MENU);
		}
	}
};