#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"

void ExitGameSystem::Update(Level& level, float dt)
{
	if (!cursor_and_keys_.window_close_button_pressed_this_frame) { return; }
	if (!is_in_level_editing_ || level.editor.Empty())
	{
		globals.render_window.close();
	}
	CreateBlockingPopupMenu(level, level.GetSize(), "Save changes before exit?", { { "Save", [&]() { level.SaveToFile(); globals.render_window.close(); }, sf::Keyboard::Unknown }, { "Discard", [&]() { level.editor.UndoAll(); level.SaveToFile(); globals.render_window.close(); }, sf::Keyboard::Unknown }, { "Cancel", [&]() {}, sf::Keyboard::Escape } }, {});
}