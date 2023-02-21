#include <functional>

class MenuEscapeSystem : public GameSystem
{
private:
	std::function<void()> go_to_last_menu_;

public:
	using GameSystem::GameSystem;
	void Give(std::function<void()> go_to_last_menu)
	{
		go_to_last_menu_ = go_to_last_menu;
	}
	void Update(Level& level, float dt)
	{
		if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape] && IsMenu(active_level_id_))
		{
			go_to_last_menu_();
		}
	}
};