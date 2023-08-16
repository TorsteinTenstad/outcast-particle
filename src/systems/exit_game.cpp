#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"

bool save_and_quit_request = false;
bool discard_and_quit_request = false;

void ExitGameSystem::Update(Level& level, float dt)
{
	if (save_and_quit_request)
	{
		if (level.GetMode() == EDIT_MODE)
		{
			level.SaveToFile();
		}
		globals.render_window.close();
		return;
	}
	if (discard_and_quit_request)
	{
		if (level.GetMode() != EDIT_MODE)
		{
			level.LoadFromFile();
			std::cout << level.editor.IsEmpty() << std::endl;
		}
		level.editor.UndoAll();
		level.SaveToFile();
		globals.render_window.close();
		return;
	}

	if (!cursor_and_keys_.window_close_button_pressed_this_frame) { return; }

	if (level.editor.IsEmpty() || !is_in_level_editing_)
	{
		globals.render_window.close();
	}

	std::function<void(void)> discard_function = []() {
		discard_and_quit_request = true;
	};

	std::function<void(void)> save_function = []() {
		save_and_quit_request = true;
	};

	CreateBlockingPopupMenu(level, level.GetSize(), "Save changes before exit?", { { "Save", save_function, sf::Keyboard::Unknown }, { "Discard", discard_function, sf::Keyboard::Unknown }, { "Cancel", []() {}, sf::Keyboard::Escape } }, {}, level.GetScale());
}