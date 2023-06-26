#include "entity_creation.hpp"
#include "systems/_pure_DO_systems.hpp"

void ExitGameSystem::Update(Level& level, float dt)
{
	if (!cursor_and_keys_.window_close_button_pressed_this_frame) { return; }
	if (level.editor.IsEmpty() || !is_in_level_editing_)
	{
		globals.render_window.close();
	}
	CreateBlockingPopupMenu(level, level.GetSize(), "Save changes before exit?", { { "Save", [&]() {
																						globals.render_window.close();
																						level.SaveToFile();
																					},
																					   sf::Keyboard::Unknown },
																					 { "Discard", [&]() {
																						  level.DiscardChanges();
																						  globals.render_window.close();
																					  },
																						 sf::Keyboard::Unknown },
																					 { "Cancel", [&]() {}, sf::Keyboard::Escape } },
		{});
}