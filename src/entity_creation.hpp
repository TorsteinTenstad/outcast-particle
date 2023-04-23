#pragma once
#include "level.hpp"
#include <string>
#include <vector>

typedef std::tuple<int, sf::Vector2f> EntityHandle;
typedef std::tuple<std::vector<int>, sf::Vector2f> EntitiesHandle;
typedef std::function<EntityHandle(sf::Vector2f)> EntityCreator;
typedef std::function<EntitiesHandle(sf::Vector2f)> EntitiesCreator;

EntitiesCreator AdaptToEntitiesCreator(EntityCreator EntityCreator);
EntitiesHandle AdaptToEntitiesHandle(EntityHandle EntityHandle);

EntitiesHandle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesCreator> entities_creators, float spacing);
EntitiesHandle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_creators, float spacing, UiOrigin ui_origin = Center);
EntitiesHandle HorizontalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_handles, float spacing, UiOrigin ui_origin = Center);
EntityHandle CreateText(ECSScene& level, sf::Vector2f position, std::string text, unsigned int text_size);
EntityHandle CreateButtonTemplate(ECSScene& level, sf::Vector2f position, sf::Vector2f size);
EntityHandle CreateSizedButtonTemplate(ECSScene& level, sf::Vector2f position);
EntityHandle CreateButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> on_click, std::string text, unsigned textsize);
EntityHandle CreateMenuButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
EntityHandle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key);
EntitiesHandle CreateKeyConfigButton(ECSScene& level, sf::Vector2f position, sf::Keyboard::Key* key);
EntityHandle CreateTexturedRectangle(ECSScene& level, sf::Vector2f position, sf::Vector2f size, int draw_priority, std::string image_path, bool scale_to_fit);
EntityHandle CreateTimerButton(ECSScene& level, sf::Vector2f position);
EntityHandle CreateMenuNavigator(ECSScene& level, float buttons_height_in_block_size = 2.f);
EntitiesHandle CreateOptionsButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
EntitiesHandle CreateSliderButton(ECSScene& level, sf::Vector2f position, int* f);
EntityHandle CreateScrollingText(ECSScene& level, sf::Vector2f position, std::string text);
EntityHandle CreateStatsBadge(ECSScene& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle);
int CreateScreenWideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority);

#include "entity_creation.tpp"
