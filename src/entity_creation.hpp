#pragma once
#include "level.hpp"
#include <string>
#include <vector>
typedef std::tuple<std::vector<int>, sf::Vector2f> entities_handle;
typedef std::function<std::tuple<std::vector<int>, sf::Vector2f>(sf::Vector2f)> entities_creator;

entities_handle VerticalEntityLayout(Level& level, sf::Vector2f position, std::vector<entities_creator> entities_creators, float spacing);
entities_handle AddText(Level& level, sf::Vector2f position, std::string text, unsigned int text_size);
std::vector<int> AddMenuButton(Level& level, std::function<void(void)> on_click, float pos_x, float pos_y, std::string button_text);
entities_handle AddButtonList(Level& level, sf::Vector2f position, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts, std::vector<sf::Keyboard::Key> shortcut_keys = {}, float x_scale = 1, float y_scale = 1, UiOrigin ui_origin = CenterCenter);
std::vector<int> AddKeyConfigButton(Level& level, sf::Keyboard::Key* key, sf::Vector2f position);
std::vector<int> AddOptionsButton(Level& level, std::function<void(void)> on_click, std::string button_text, sf::Vector2f position);
std::vector<int> AddSliderButton(Level& level, int* f, sf::Vector2f button_position);
int AddScrollingText(Level& level, sf::Vector2f position, std::string text);
entities_handle AddStatsBadge(Level& level, sf::Vector2f position, int coin_number);
int CreateScreenwideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority);

#include "entity_creation.tpp"
