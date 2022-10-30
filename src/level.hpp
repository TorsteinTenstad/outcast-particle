#pragma once
#include "PCH.hpp"
#include "comp_draw_info.hpp"
#include "cursor_and_keys.hpp"
#include "sys_acceleration.hpp"
#include "sys_display_velocity.hpp"
#include "sys_edit_mode.hpp"
#include "sys_electric_force.hpp"
#include "sys_force.hpp"
#include "sys_player.hpp"
#include "sys_sfml_render.hpp"
#include "sys_velocity.hpp"
#include <map>

class Level
{
private:
	std::map<int, DrawInfo> draw_info_;
	std::map<int, Position> position_;
	std::map<int, Velocity> velocity_;
	std::map<int, Acceleration> acceleration_;
	std::map<int, ReceivedForces> received_forces_;
	std::map<int, Player> player_;
	std::map<int, Charge> charge_;
	std::map<int, Draggable> draggable_;
	std::map<int, Draggable> draggable_;
	std::map<int, Radius> radius_;
	std::map<int, WidthAndHight> width_and_hight_;
	std::map<int, Boarder> boarder_;

	EditModeSystem edit_mode_system_;
	DisplayVelocitySystem display_velocity_system_;
	PlayerSystem player_system_;
	ElectricForceSystem electric_force_system_;
	ForceSystem force_system_;
	AccelerationSystem acceleration_system_;
	VelocitySystem velocity_system_;
	SFMLRenderSystem render_system_;

	int next_available_entity_id_ = 0;
	int CreateEntityId();

public:
	void LoadEntitiesFromFile(std::string path);
	void SaveEntitiesToFile(std::string path);
	void Update(CursorAndKeys cursor_and_keys, float dt);
	int AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	int AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	int AddParticleEntity(float pos_x, float pos_y, float charge);

	int AddButton(float pos_x, float pos_y, float width, float hight, std::string path);
};