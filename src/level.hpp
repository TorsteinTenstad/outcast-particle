#pragma once
#include "PCH.hpp"
#include "components/area.hpp"
#include "components/draw_info.hpp"
#include "components/level_button.hpp"
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"
#include "utils.hpp"
#include <experimental/filesystem>
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
	std::map<int, WidthAndHight>,
	std::map<int, Boarder>,
	std::map<int, LevelButton>>
	ComponentMap;

class Level
{
private:
	static int next_available_entity_id_;
	std::map<const char*, ComponentMap> components_;

	template <class Component>
	void RegisterComponent();

public:
	template <class Component>
	std::map<int, Component>& GetComponent();
	void SaveToFolder(std::string folder_path) const;
	void LoadFromFolder(std::string folder_path);
	int CreateEntityId();
	int CopyEntity(int from_id);
	int AddParticleEntity(float pos_x, float pos_y, float charge);
	int AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	int AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	int AddLevelButton(int level, float pos_x, float pos_y, float width, float hight, std::string path);
	int AddLaser();
	int AddBlock(float pos_x, float pos_y);
};