#pragma once
#include <cassert>
#include <map>
#include <typeindex>
#include <variant>


template <class Key, class... Ts>
void MoveAllEntriesAtKey(Key key,
	std::map<std::type_index, std::variant<std::map<Key, Ts>...>>& from,
	std::map<std::type_index, std::variant<std::map<Key, Ts>...>>& to)
{
	for (auto& [type_index, from_variant] : from)
	{
		std::visit([&to, key, type_index](auto& from_map) {
			using FromMapType = std::decay_t<decltype(from_map)>;
			using ValueType = typename FromMapType::mapped_type;
			auto it = from_map.find(key);
			if (it == from_map.end())
			{
				return;
			}
			auto to_variant_it = to.find(type_index);
			if (to_variant_it == to.end())
			{
				auto [inserted_it, inserted] = to.emplace(type_index, std::map<Key, ValueType> {});
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
}