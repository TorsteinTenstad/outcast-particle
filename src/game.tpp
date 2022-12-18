#include "game.hpp"

template <class System>
System& Game::GetGameSystem()
{
	assert(system_ids_.count(typeid(System)) > 0);
	int system_id = system_ids_[typeid(System)];
	if (game_systems_.count(system_id) > 0)
	{
		return *(static_cast<System*>(game_systems_[system_id].get()));
	}
	else
	{
		assert(physics_game_systems_.count(system_id) > 0);
		return *(static_cast<System*>(physics_game_systems_[system_id].get()));
	}
}

template <class System>
System& Game::RegisterGameSystem(std::map<int, std::unique_ptr<GameSystem>>& map)
{
	int id = next_available_system_id_++;
	system_ids_[typeid(System)] = id;
	map[id] = std::move(std::make_unique<System>(active_mode_, cursor_and_keys_));
	return GetGameSystem<System>();
}

template <class System>
System& Game::RegisterGameSystem()
{
	return RegisterGameSystem<System>(game_systems_);
}

template <class System>
System& Game::RegisterPhysicsGameSystem()
{
	return RegisterGameSystem<System>(physics_game_systems_);
}