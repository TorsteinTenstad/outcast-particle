#pragma once
#include "PCH.hpp"
#include "comp_draw_info.hpp"
#include "sys_acceleration.hpp"
#include "sys_electric_force.hpp"
#include "sys_event.hpp"
#include "sys_force.hpp"
#include "sys_keyboard_force.hpp"
#include "sys_render.hpp"
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

	EventSystem event_system_;
	KeyboardForceSystem keyboard_force_system_;
	ElectricForceSystem electric_force_system_;
	ForceSystem force_system_;
	AccelerationSystem acceleration_system_;
	VelocitySystem velocity_system_;
	RenderSystem render_system_;

	int next_available_entity_id_ = 0;
	int CreateEntityId();

public:
	Level();
	void LoadEntitiesFromFile(std::string path);
	void SaveEntitiesToFile(std::string path);
	void Update(float dt);
	void AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	void AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	void AddParticleEntity(float pos_x, float pos_y, float charge);
};