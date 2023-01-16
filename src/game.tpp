#include "game.hpp"

template <class System>
System& Game::GetGameSystem()
{
	assert(type_to_system_id_.count(typeid(System)) > 0);
	int system_id = type_to_system_id_[typeid(System)];
	assert(game_systems_.count(system_id) > 0);
	return *(static_cast<System*>(game_systems_[system_id].get()));
}

template <class System>
System& Game::RegisterGameSystem(std::vector<int>& category)
{
	int id = next_available_system_id_++;
	type_to_system_id_[typeid(System)] = id;
	category.push_back(id);
	game_systems_[id] = std::move(std::make_unique<System>(active_level_id_, active_mode_, cursor_and_keys_));
	return GetGameSystem<System>();
}

template <class System>
System& Game::RegisterGameSystem()
{
	return RegisterGameSystem<System>(game_system_ids_);
}

template <class System>
System& Game::RegisterPhysicsGameSystem()
{
	return RegisterGameSystem<System>(physics_game_system_ids_);
}