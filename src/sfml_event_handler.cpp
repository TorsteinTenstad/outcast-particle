
#include "sfml_event_handler.hpp"
#include "SFML/Window/Event.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"

void SFMLEventHandler::Update(CursorAndKeys& cursor_and_keys)
{
	cursor_and_keys.ResetFrameEvents();

	cursor_and_keys.last_frame_screen_space_cursor_position = cursor_and_keys.screen_space_cursor_position;
	cursor_and_keys.screen_space_cursor_position = sf::Mouse::getPosition(globals.render_window);

	cursor_and_keys.cursor_moved_this_frame = cursor_and_keys.last_frame_screen_space_cursor_position != cursor_and_keys.screen_space_cursor_position;

	cursor_and_keys.last_frame_cursor_position = cursor_and_keys.cursor_position;
	cursor_and_keys.cursor_position = globals.render_window.mapPixelToCoords(cursor_and_keys.screen_space_cursor_position);

	sf::Event event;
	if (!globals.render_window.hasFocus())
	{
		cursor_and_keys.ResetAllKeysAndButtons();
	}
	while (globals.render_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			globals.render_window.close();
		}
		if (event.type == sf::Event::KeyPressed && event.key.code != sf::Keyboard::Unknown)
		{
			cursor_and_keys.key_pressed_this_frame[event.key.code] = !cursor_and_keys.key_down[event.key.code];
			cursor_and_keys.key_down[event.key.code] = true;
		}
		if (event.type == sf::Event::KeyReleased && event.key.code != sf::Keyboard::Unknown)
		{
			cursor_and_keys.key_released_this_frame[event.key.code] = true;
			cursor_and_keys.key_down[event.key.code] = false;
		}
		if (event.type == sf::Event::MouseWheelScrolled)
		{
			cursor_and_keys.mouse_wheel_delta = event.mouseWheelScroll.delta;
			cursor_and_keys.cursor_moved_this_frame = true;
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			cursor_and_keys.mouse_button_pressed_this_frame[event.mouseButton.button] = true;
			cursor_and_keys.mouse_button_down[event.mouseButton.button] = true;
			cursor_and_keys.mouse_button_last_pressed_position[event.mouseButton.button] = cursor_and_keys.cursor_position;
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			cursor_and_keys.mouse_button_released_this_frame[event.mouseButton.button] = true;
			cursor_and_keys.mouse_button_down[event.mouseButton.button] = false;
			cursor_and_keys.mouse_button_last_released_position[event.mouseButton.button] = cursor_and_keys.cursor_position;
		}
	}
}