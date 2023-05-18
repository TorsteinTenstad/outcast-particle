#pragma once
#include "level.hpp"
#include <string>
#include <vector>

typedef std::tuple<Entity, sf::Vector2f> EntityHandle;
typedef std::tuple<std::vector<Entity>, sf::Vector2f> EntitiesHandle;
typedef std::function<EntityHandle(sf::Vector2f)> EntityCreator;
typedef std::function<EntitiesHandle(sf::Vector2f)> EntitiesCreator;

template <class T>
Entity GetEntity(T& handle)
{
	return std::get<Entity>(handle);
}
template <class T>
std::vector<Entity> GetEntities(T& handle)
{
	return std::get<std::vector<Entity>>(handle);
}
template <class T>
sf::Vector2f GetSize(T& handle)
{
	return std::get<sf::Vector2f>(handle);
}

template <class... EntityHandles>
EntitiesHandle ToEntitiesHandle(EntityHandle handle, EntityHandles... handles)
{
	sf::Vector2f total_size = sf::Vector2f(
		std::max(GetSize(handle).x, GetSize(handles).x...),
		std::max(GetSize(handle).y, GetSize(handles).y...));

	std::vector<Entity> entities = std::vector<Entity>({ GetEntity(handle), GetEntity(handles)... });
	return { entities, total_size };
}

EntitiesHandle ToEntitiesHandle(EntityHandle EntityHandle);

EntitiesCreator AdaptToEntitiesCreator(EntityCreator EntityCreator);

EntitiesHandle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesCreator> entities_creators, float spacing);
EntitiesHandle VerticalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_creators, float spacing, UiOrigin ui_origin = Center);
EntitiesHandle HorizontalEntityLayout(ECSScene& level, sf::Vector2f position, std::vector<EntitiesHandle> entities_handles, float spacing, UiOrigin ui_origin = Center);
EntityHandle CreateText(ECSScene& level, sf::Vector2f position, std::string content, unsigned int text_size = 120);
EntityHandle CreateScrollingText(ECSScene& level, sf::Vector2f position, std::string content, unsigned int text_size = 120);
EntityHandle CreateButtonTemplate(ECSScene& level, sf::Vector2f position, sf::Vector2f size);
EntityHandle CreateMouseEventButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size);
EntityHandle CreateSizedButtonTemplate(ECSScene& level, sf::Vector2f position);
EntityHandle CreateButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> on_click, std::string text, unsigned textsize);
EntityHandle CreateMenuButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
EntityHandle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key);
EntitiesHandle CreateKeyConfigButton(ECSScene& level, sf::Vector2f position, sf::Keyboard::Key* key);
EntityHandle CreateTexturedRectangle(ECSScene& level, sf::Vector2f position, sf::Vector2f size, int draw_priority, std::string image_path, bool tile);
EntityHandle CreateTimerButton(ECSScene& level, sf::Vector2f position);
EntityHandle CreateMenuNavigator(ECSScene& level, float buttons_height_in_block_size = 2.f);
EntitiesHandle CreateOptionsButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
EntityHandle CreateScreenWideBlur(ECSScene& level, sf::Vector2f level_size, int draw_priority);
EntitiesHandle CreateConfirmMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::function<void(void)> confirm_function);
EntitiesHandle CreateBlockingPopupMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::vector<std::pair<std::string, std::function<void(void)>>> button_functions, EntitiesHandle middle_entities, bool vertical = false);
EntitiesHandle CreateSliderButton(ECSScene& level, sf::Vector2f position, int* f);
EntityHandle CreateStatsBadge(ECSScene& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle);
Entity CreateScreenWideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority);

#include "entity_creation.tpp"