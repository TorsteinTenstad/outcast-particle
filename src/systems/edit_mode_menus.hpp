#pragma once
#include "components/blueprint_menu_item.hpp"
#include "components/not_serialized.hpp"
#include "level.hpp"

template <class MenuEntityTag, class ToggleButton>
class MenuHelper
{
public:
	virtual void Create(Level& level) = 0;
	void Open(Level& level)
	{
		auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponents<MenuEntityTag, NotSerialized, EditModeUIEntity>(entity); });
		Create(level);
	}

	void Update(Level& level)
	{
		if (level.GetEntitiesWith<ToggleButton, StickyButtonDown>().size() > 0 && !IsOpen(level)) { Open(level); }
		if (level.GetEntitiesWith<ToggleButton, StickyButtonDown>().size() == 0 && IsOpen(level)) { Close(level); }
	}

	void Close(Level& level)
	{
		level.DeleteEntitiesWith<MenuEntityTag>();
		for (auto [entity, toggle_button, sticky_button_down] : level.GetEntitiesWith<ToggleButton, StickyButtonDown>())
		{
			level.RemoveComponent<StickyButtonDown>(entity);
		}
	}

	void Toggle(Level& level)
	{
		IsOpen(level) ? Close(level) : Open(level);
	}

	bool IsOpen(Level& level)
	{
		return level.GetEntitiesWith<MenuEntityTag>().size() > 0;
	}
};

class ToggleBlueprintMenuButton
{};
class BlueprintMenu : public MenuHelper<BlueprintMenuItem, ToggleBlueprintMenuButton>
{
public:
	void Create(Level& level);

	std::optional<Entity> HandleSelection(Level& level);
};

class ToggleMusicMenuButton
{};
class MusicMenuItem
{};
class MusicMenu : public MenuHelper<MusicMenuItem, ToggleMusicMenuButton>
{
public:
	void Create(Level& level);
};

class TogglePlayerMenuButton
{};
class PlayerMenuItem
{};
class PlayerMenu : public MenuHelper<PlayerMenuItem, TogglePlayerMenuButton>
{
public:
	void Create(Level& level);
};
