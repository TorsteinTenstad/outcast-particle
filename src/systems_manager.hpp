#include "PCH.hpp"
#include "acceleration_system.hpp"
#include "draw_info.hpp"
#include "electric_force_system.hpp"
#include "event_system.hpp"
#include "force_system.hpp"
#include "render_system.hpp"
#include "velocity_system.hpp"
#include <map>

class SystemsManager
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
	AccelerationSystem acceleration_system_;
	VelocitySystem velocity_system_;
	RenderSystem render_system_;
	ElectricForceSystem electric_force_system_;
	ForceSystem force_system_;

	int next_available_entity_id_ = 0;
	int CreateEntityId();

public:
	SystemsManager();
	void LoadEntitiesFromFile(std::string path);
	void SaveEntitiesFromFile(std::string path);
	void Update(sf::RenderWindow& window, float dt);
	void AddPlayerEntity(float pos_x, float pos_y, float vel_x, float vel_y, float charge);
	void AddParticleEntity(float pos_x, float pos_y, float charge);
};