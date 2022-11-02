#pragma once
#include "PCH.hpp"
#include "comp_area.hpp"
#include "comp_draw_info.hpp"
#include "comp_level_button.hpp"
#include "comp_mouse_interactions.hpp"
#include "comp_physics.hpp"
#include "comp_player.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"

class Level
{
private:
public:
	std::map<int, DrawInfo> draw_info_;
	std::map<int, Position> position_;
	std::map<int, Velocity> velocity_;
	std::map<int, Acceleration> acceleration_;
	std::map<int, ReceivedForces> received_forces_;
	std::map<int, Player> player_;
	std::map<int, Charge> charge_;
	std::map<int, Draggable> draggable_;
	std::map<int, ClickedOn> clicked_on_;
	std::map<int, Radius> radius_;
	std::map<int, WidthAndHight> width_and_hight_;
	std::map<int, Boarder> boarder_;
	std::map<int, LevelButton> level_button_;

	int CreateEntityId();
	void LoadEntitiesFromFile(std::string path);
	void SaveEntitiesToFile(std::string path);
	int AddParticleEntity(float pos_x, float pos_y, float charge);
	int AddMovingParticleEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	int AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	int AddLevelButton(int level, float pos_x, float pos_y, float width, float hight, std::string path);
	int AddLaser();
};