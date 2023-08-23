#pragma once
#include "components/blueprint_menu_item.hpp"
#include "components/not_serialized.hpp"
#include "level.hpp"

template <class MenuEntityTag>
class MenuHelper
{
public:
	virtual void Create(Level& level) = 0;
	void Open(Level& level)
	{
		auto e = EntityCreationObserver(level, [](ECSScene& level, Entity entity) { level.AddComponents<MenuEntityTag, NotSerialized, EditModeUIEntity>(entity); });
		Create(level);
	}

	void Close(Level& level)
	{
		level.DeleteEntitiesWith<MenuEntityTag>();
	}

	void Toggle(Level& level)
	{
		if (level.GetEntitiesWith<MenuEntityTag>().size() > 0)
		{
			Close(level);
		}
		else
		{
			Open(level);
		}
	}
};

class BlueprintMenu : public MenuHelper<BlueprintMenuItem>
{
public:
	void Create(Level& level);

	//Handle selection from blueprint menu. Return any selected entity.
	std::optional<Entity> Update(Level& level);
};

class MusicMenu : public MenuHelper<MusicMenuItem>
{
public:
	void Create(Level& level);
};
