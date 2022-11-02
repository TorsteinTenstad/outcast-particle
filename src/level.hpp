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
	int AddBlock(float pos_x, float pos_y);
};