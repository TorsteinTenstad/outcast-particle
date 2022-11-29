#pragma once
#include "Components/collision.hpp"
#include "Components/goal.hpp"
#include "Components/intersection.hpp"
#include "Components/kill_on_intersection.hpp"
#include "Components/sound_info.hpp"
#include "PCH.hpp"
#include "components/area.hpp"
#include "components/blueprint_menu_item.hpp"
#include "components/draw_info.hpp"
#include "components/level_button.hpp"
#include "components/mouse_interactions.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/player_behaviours.hpp"
#include "components/tag.hpp"
#include "components/trail.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"
#include <typeindex>
#include <variant>

typedef std::variant<
	std::map<int, DrawInfo>,
	std::map<int, ChargeDependentDrawInfo>,
	std::map<int, OrientationDependentDrawInfo>,
	std::map<int, Position>,
	std::map<int, Velocity>,
	std::map<int, Acceleration>,
	std::map<int, ReceivedForces>,
	std::map<int, Player>,
	std::map<int, PlayerBehaviours>,
	std::map<int, Collision>,
	std::map<int, Charge>,
	std::map<int, Editable>,
	std::map<int, BlueprintMenuItem>,
	std::map<int, ClickedOn>,
	std::map<int, Radius>,
	std::map<int, WidthAndHeight>,
	std::map<int, Border>,
	std::map<int, Trail>,
	std::map<int, Tag>,
	std::map<int, KillOnIntersection>,
	std::map<int, Goal>,
	std::map<int, ElectricField>,
	std::map<int, MagneticField>,
	std::map<int, Intersection>,
	std::map<int, SoundInfo>,
	std::map<int, LevelButton>>
	ComponentMap;

class Level
{
private:
	std::string savefile_path_;

	static int next_available_entity_id_;
	std::map<std::type_index, ComponentMap> components_;

	template <class Component>
	void RegisterComponent();

public:
	bool editable = false;
	bool edit_mode = false;
	std::string name = "Untitled";
	sf::Vector2f size = sf::Vector2f(1920 * 4, 1080 * 4);
	std::map<float, float> screen_size_shake_animation;
	sf::Vector2f post_shake_size;

	Level();

	template <class Component>
	std::map<int, Component>& GetComponent();

	int CreateEntityId();
	int CopyEntity(int from_id);
	void DeleteEntity(int id);
	int AddLevelButton(std::function<void(void)> on_click, float pos_x, float pos_y, float width, float height, std::string image_path);
	void SaveToFile();
	void LoadFromFile();

	void SaveToFile(std::string savefile_path);
	void LoadFromFile(std::string savefile_path);
	int AddBlueprint(std::string tag);
};