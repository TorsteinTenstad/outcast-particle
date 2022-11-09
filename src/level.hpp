#pragma once
#include "Components/goal.hpp"
#include "Components/intersection.hpp"
#include "Components/kill_on_intersection.hpp"
#include "PCH.hpp"
#include "components/area.hpp"
#include "components/draw_info.hpp"
#include "components/level_button.hpp"
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"
#include <typeindex>
#include <variant>

typedef std::variant<
	std::map<int, DrawInfo>,
	std::map<int, Position>,
	std::map<int, Velocity>,
	std::map<int, Acceleration>,
	std::map<int, ReceivedForces>,
	std::map<int, Player>,
	std::map<int, Charge>,
	std::map<int, Draggable>,
	std::map<int, ClickedOn>,
	std::map<int, Radius>,
	std::map<int, WidthAndHeight>,
	std::map<int, Border>,
	std::map<int, KillOnIntersection>,
	std::map<int, Goal>,
	std::map<int, Intersection>,
	std::map<int, LevelButton>>
	ComponentMap;

class Level
{
private:
	static int next_available_entity_id_;
	std::map<std::type_index, ComponentMap> components_;

	template <class Component>
	void RegisterComponent();

public:
	template <class Component>
	std::map<int, Component>& GetComponent();

	int CreateEntityId();
	int CopyEntity(int from_id);
	void DeleteEntity(int id);
	int AddParticleEntity(float pos_x, float pos_y, float charge);
	int AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	int AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge, float player_force);
	int AddLevelButton(int level, float pos_x, float pos_y, float width, float height, std::string path);
	int AddLaser(float pos_x, float pos_y, float width, float height);
	int AddBlock(float pos_x, float pos_y);
	int AddGoal();
};