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
EntityHandle CreateScrollWindow(ECSScene& level, sf::Vector2f position, sf::Vector2f width_and_height, float entity_height);
EntityHandle CreateText(ECSScene& level, sf::Vector2f position, std::string content, unsigned int text_size = 120, std::optional<sf::Vector2f> layout_size = std::nullopt);
EntityHandle CreateTextPopup(ECSScene& level, sf::Vector2f position, std::string content, float duration, bool has_backdrop);
EntityHandle CreateScrollingText(ECSScene& level, sf::Vector2f position, std::string content, unsigned int text_size = 120);
EntityHandle CreateButtonTemplate(ECSScene& level, sf::Vector2f position, sf::Vector2f size);
EntityHandle CreateMouseEventButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size);
EntityHandle CreateSizedButtonTemplate(ECSScene& level, sf::Vector2f position);
EntityHandle CreateButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> on_click, std::string text, unsigned textsize);
EntityHandle CreateMenuButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
EntityHandle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key);
EntityHandle CreateNavigatorButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> button_function, std::string button_text, sf::Keyboard::Key shortcut_key, sf::Vector2f size);
EntitiesHandle CreateKeyConfigButton(ECSScene& level, sf::Vector2f position, sf::Keyboard::Key* key);
EntityHandle CreateTexturedRectangle(ECSScene& level, sf::Vector2f position, sf::Vector2f size, int draw_priority, std::string image_path, bool tile);
EntityHandle CreateCanDisableButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> button_function, std::string button_text, unsigned text_size, std::function<bool(void)> deactivate_function);
EntitiesHandle CreateCanDisableButtonWithIcon(ECSScene& level, sf::Vector2f position, sf::Vector2f size, std::function<void(void)> button_function, std::string icon_path, unsigned text_size, std::function<bool(void)> deactivate_function);
EntityHandle CreateMenuNavigator(ECSScene& level);
EntitiesHandle CreateOptionsButton(ECSScene& level, sf::Vector2f position, std::function<void(void)> on_click, std::string button_text);
EntityHandle CreateScreenWideBlur(ECSScene& level, sf::Vector2f level_size, int draw_priority);
EntitiesHandle CreateConfirmMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::function<void(void)> confirm_function, float scale = 1);
EntitiesHandle CreateBlockingInformationMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::string body = "", float scale = 1);
EntitiesHandle CreateBlockingPopupMenu(ECSScene& level, sf::Vector2f level_size, std::string title, std::vector<std::tuple<std::string, std::function<void(void)>, sf::Keyboard::Key>> button_info, EntitiesHandle middle_entities, float scale = 1);
EntitiesHandle CreateSliderButton(ECSScene& level, sf::Vector2f position, sf::Vector2f size, int* f);
EntityHandle CreateStatsBadge(ECSScene& level, sf::Vector2f position, int coin_number, sf::Uint8 alpha, std::string text, bool twinkle, float scale = 1);
Entity CreateScreenWideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority);

#include "entity_creation.tpp"