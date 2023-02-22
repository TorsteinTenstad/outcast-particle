#include "systems/menu_escape.hpp"
#include "utils.hpp"

void MenuEscapeSystem::Give(std::function<void()> go_to_last_menu)
{
	go_to_last_menu_ = go_to_last_menu;
}
void MenuEscapeSystem::Update(Level& level, float dt)
{
	if (cursor_and_keys_.key_pressed_this_frame[sf::Keyboard::Escape] && IsMenu(active_level_id_))
	{
		go_to_last_menu_();
	}
}