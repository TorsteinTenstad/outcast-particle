#pragma once
#include "level.hpp"
#include <string>
#include <vector>

typedef std::tuple<int, sf::Vector2f> entity_handle;
typedef std::tuple<std::vector<int>, sf::Vector2f> entities_handle;
typedef std::function<entity_handle(sf::Vector2f)> entity_creator;
typedef std::function<entities_handle(sf::Vector2f)> entities_creator;

entities_creator AdaptToEntitiesCreator(entity_creator entity_creator);
entities_handle AdaptToEntitiesHandle(entity_handle entity_handle);

entities_handle VerticalEntityLayout(Level& level, sf::Vector2f position, std::vector<entities_creator> entities_creators, float spacing);
entities_handle VerticalEntityLayout(Level& level, sf::Vector2f position, std::vector<entities_handle> entities_creators, float spacing);
entity_handle CreateText(Level& level, sf::Vector2f position, std::string text, unsigned int text_size);
entity_handle CreateMenuButton(Level& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
entity_handle CreateNavigatorButton(Level& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key);
entities_handle CreateButtonList(Level& level, sf::Vector2f position, std::vector<std::function<void(void)>> button_functions, std::vector<std::string> button_texts, std::vector<sf::Keyboard::Key> shortcut_keys = {}, float x_scale = 1, float y_scale = 1, UiOrigin ui_origin = CenterCenter);
entities_handle CreateKeyConfigButton(Level& level, sf::Vector2f position, sf::Keyboard::Key* key);
entity_handle CreateTimerButton(ECSScene& level, sf::Vector2f position);
entities_handle CreateOptionsButton(Level& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
entities_handle CreateSliderButton(Level& level, sf::Vector2f position, int* f);
entity_handle CreateScrollingText(Level& level, sf::Vector2f position, std::string text);
entity_handle CreateStatsBadge(Level& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle);
int CreateScreenWideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority);

#include "entity_creation.tpp"
