#pragma once
#include <cassert>
#include <map>
#include <typeindex>
#include <variant>

template <class Key, class... Ts>
void MoveAllComponentsAtKey(Key key,
	std::map<std::type_index, std::variant<std::map<Key, Ts>...>>& from_container,
	std::map<std::type_index, std::variant<std::map<Key, Ts>...>>& to_container)
{
	for (auto& [type_index, from_variant] : from_container)
	{
		MoveComponentAtKey(key, type_index, from_variant, to_container);
	}
}

template <class Key, class... Ts>
void MoveComponentAtKey(Key key, std::type_index component_type_index,
	std::variant<std::map<Key, Ts>...>& from_variant,
	std::map<std::type_index, std::variant<std::map<Key, Ts>...>>& to_container)
{
	std::visit([&to_container, key, component_type_index](auto& from_map) {
		using FromMapType = std::decay_t<decltype(from_map)>;
		using ValueType = typename FromMapType::mapped_type;
		auto it = from_map.find(key);
		if (it == from_map.end())
		{
			return;
		}
		auto to_variant_it = to_container.find(component_type_index);
		if (to_variant_it == to_container.end())
		{
			auto [inserted_it, inserted] = to_container.emplace(component_type_index, std::map<Key, ValueType> {});
			to_variant_it = inserted_it;
			assert(inserted);
		}
		auto& to_map = std::get<std::map<Key, ValueType>>(to_variant_it->second);
		auto [to_it, inserted] = to_map.emplace(key, it->second);
		assert(inserted);
		from_map.erase(it);
	},
		from_variant);
}