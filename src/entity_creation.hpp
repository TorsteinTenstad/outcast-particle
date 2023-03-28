#pragma once
#include "level.hpp"
#include <string>
#include <vector>
typedef std::tuple<std::vector<int>, sf::Vector2f> entities_handle;
typedef std::function<std::tuple<std::vector<int>, sf::Vector2f>(sf::Vector2f)> entities_creator;

entities_handle VerticalEntityLayout(Level& level, sf::Vector2f position, std::vector<entities_creator> entities_creators, float spacing);
entities_handle CreateText(Level& level, sf::Vector2f position, std::string text, unsigned int text_size);
entities_handle CreateMenuButton(Level& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
entities_handle CreateNavigatorButton(Level& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key);
entities_handle CreateButtonList(Level& level, sf::Vector2f position, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts, std::vector<sf::Keyboard::Key> shortcut_keys = {}, float x_scale = 1, float y_scale = 1, UiOrigin ui_origin = CenterCenter);
entities_handle CreateKeyConfigButton(Level& level, sf::Vector2f position, sf::Keyboard::Key* key);
entities_handle CreateOptionsButton(Level& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
entities_handle CreateSliderButton(Level& level, sf::Vector2f position, int* f);
entities_handle CreateScrollingText(Level& level, sf::Vector2f position, std::string text);
entities_handle CreateStatsBadge(Level& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha);
int CreateScreenwideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority);

#include "entity_creation.tpp"
