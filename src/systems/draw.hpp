#pragma once
#include "constants.hpp"
#include "game_system.hpp"
#include "level.hpp"

class DrawSystem : public GameSystem
{
private:
	sf::RectangleShape background_;
	sf::Texture background_texture_;
	sf::RectangleShape grid_background_;
	sf::Texture grid_background_texture_;

public:
	DrawSystem()
	{
		background_texture_.loadFromFile("content\\textures\\background.png");
		background_texture_.setRepeated(true);
		background_.setTexture(&background_texture_);
		grid_background_texture_.loadFromFile("content\\textures\\grid.png");
		grid_background_texture_.setRepeated(true);
		grid_background_.setTexture(&grid_background_texture_);
	}
	void Update(CursorAndKeys& cursor_and_keys, Level& level, float dt)
	{
		globals.render_window.clear();

		sf::Vector2f background_size = level.size * MAX_SCREEN_SIZE_SHAKE;
		background_.setSize(background_size);
		background_.setPosition(-(background_size - level.size) / 2.f);
		globals.render_window.draw(background_);

		/*if (level.edit_mode)
		{
			grid_background_.setSize(background_size);
			grid_background_.setTextureRect(sf::IntRect(0, 0, background_size.x, background_size.y));
			globals.render_window.draw(grid_background_);
		}*/

		for (auto [draw_priority, drawables] : level.drawables)
		{
			for (auto drawable : drawables)
			{
				globals.render_window.draw(*drawable);
			}
		}
		level.drawables.clear();
	}
};