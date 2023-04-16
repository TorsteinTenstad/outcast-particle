#pragma once
#include <cassert>
#include <map>
#include <memory>
#include <typeindex>

typedef int Key;

class ComponentMap
{
public:
	virtual ~ComponentMap() = 0;
	virtual void Clear() = 0;
	virtual bool Exists(Key key) = 0;
	virtual void DeleteComponent(Key key) = 0;
	virtual void CopyComponent(Key from_key, Key to_key) = 0;
	virtual void CopyComponent(Key from_key, ComponentMap* to_other_map) = 0;
};

template <class Component>
class TypedComponentMap : ComponentMap
{
private:
	std::map<Key, Component> map_;

public:
	std::map<Key, Component>& GetMap()
	{
		return map_;
	}
	void Clear(Key key)
	{
		map_.clear();
	}
	bool Exists(Key key)
	{
		return map_.find(key) != map_.end();
	}
	void DeleteComponent(Key key)
	{
		map_.erase(key);
	}
	void CopyComponent(Key from_key, Key to_key)
	{
		auto it = map_.find(from_key);
		if (it == map_.end())
		{
			return;
		}
		auto [to_it, inserted] = map_.emplace(*it);
		assert(inserted);
	}
	void CopyComponent(Key from_key, ComponentMap* to_other_map)
	{
		auto it = map_.find(from_key);
		if (it == map_.end())
		{
			return;
		}
		assert(typeid(*this) == typeid(*to_other_map));
		TypedComponentMap<Component>* other_map = static_cast<TypedComponentMap<Component>*>(to_other_map);
		auto [to_it, inserted] = other_map->GetMap().emplace(*it);
		assert(inserted);
	}
};

class EntityContainer
{
private:
	std::map<std::type_index, std::unique_ptr<ComponentMap>> components_;

protected:
	template <class Component>
	std::map<Key, Component>& GetComponentMap()
	{
		auto it = components_.find(typeid(Component));
		if (it == components_.end())
		{
			auto [inserted_it, inserted] = components_.emplace(typeid(Component), std::make_unique < TypedComponentMap<Component>);
			assert(inserted);
			it = inserted_it;
		}
		std::unique_ptr<ComponentMap> component_map = it->second;
		return static_cast<TypedComponentMap<Component>*>(component_map.get())->GetMap();
	}

public:
	bool Exists(Key key)
	{
		for (auto& [type_index, component_map] : components_)
		{
			if (component_map->Exists(key))
			{
				return true;
			}
		}
		return false;
	}
	void Clear()
	{
		for (auto& [type_index, component_map] : components_)
		{
			component_map->Clear();
		}
	}
	void DeleteEntity(Key key)
	{
		for (auto& [type_index, component_map] : components_)
		{
			component_map->DeleteComponent(key);
		}
	}
	void CopyEntity(Key from_key, Key to_key)
	{
		for (auto& [type_index, component_map] : components_)
		{
			component_map->CopyComponent(from_key, to_key);
		}
	}
	void CopyEntity(Key from_key, EntityContainer to_other_entity_container)
	{
		for (auto& [type_index, component_map] : components_)
		{
			auto it = to_other_entity_container.components_.find(type_index);
			assert(it != to_other_entity_container.components_.end());
			ComponentMap* other_component_map = it->second.get();
			component_map->CopyComponent(from_key, other_component_map);
		}
	}
};
