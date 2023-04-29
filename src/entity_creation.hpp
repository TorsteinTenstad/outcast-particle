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

entities_handle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<entities_creator> entities_creators, float spacing);
entities_handle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<entities_handle> entities_creators, float spacing);
entities_handle CreateEntityPair(ECSScene& level, sf::Vector2f position, float spacing, std::function<entity_handle(sf::Vector2f)> creation_func_1, std::function<entity_handle(sf::Vector2f)> creation_func_2);
entity_handle CreateText(ECSScene& level, sf::Vector2f position, std::string text, unsigned int text_size);
entity_handle CreateButtonTemplate(ECSScene& level, sf::Vector2f position, sf::Vector2f size);
entity_handle CreateSizedButtonTemplate(ECSScene& level, sf::Vector2f position);
entity_handle CreateButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> on_click, std::string text, unsigned textsize);
entity_handle CreateMenuButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
entity_handle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key);
entities_handle CreateKeyConfigButton(ECSScene& level, sf::Vector2f position, sf::Keyboard::Key* key);
entity_handle CreateTexturedRectangle(ECSScene& level, sf::Vector2f position, sf::Vector2f size, int draw_priority, std::string image_path, bool scale_to_fit);
entity_handle CreateTimerButton(ECSScene& level, sf::Vector2f position);
entity_handle CreateMenuNavigator(ECSScene& level, float buttons_height_in_block_size = 2.f);
entities_handle CreateOptionsButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
entities_handle CreateConfirmMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::function<void(void)> confirm_function);
entities_handle CreateSliderButton(ECSScene& level, sf::Vector2f position, int* f);
entity_handle CreateScrollingText(ECSScene& level, sf::Vector2f position, std::string text);
entity_handle CreateStatsBadge(ECSScene& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle);
int CreateScreenWideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority);

#include "entity_creation.tpp"
