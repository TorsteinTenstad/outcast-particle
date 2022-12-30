#pragma once
#include "PCH.hpp"
#include "components/area.hpp"
#include "components/blueprint_menu_item.hpp"
#include "components/button.hpp"
#include "components/children.hpp"
#include "components/collision.hpp"
#include "components/draw_info.hpp"
#include "components/force_visualization.hpp"
#include "components/goal.hpp"
#include "components/intersection.hpp"
#include "components/kill_on_intersection.hpp"
#include "components/level_button.hpp"
#include "components/level_completion_timer.hpp"
#include "components/mouse_interactions.hpp"
#include "components/pause_menu_items.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/player_behaviours.hpp"
#include "components/shader.hpp"
#include "components/sound_info.hpp"
#include "components/tag.hpp"
#include "components/text.hpp"
#include "components/trail.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"
#include <typeindex>
#include <variant>

typedef std::variant<
	std::map<int, DrawInfo>,
	std::map<int, VelocityDependentDrawLayer>,
	std::map<int, Shader>,
	std::map<int, DrawPriority>,
	std::map<int, ChargeDependentDrawInfo>,
	std::map<int, OrientationDependentDrawInfo>,
	std::map<int, Position>,
	std::map<int, Velocity>,
	std::map<int, Acceleration>,
	std::map<int, ReceivedForces>,
	std::map<int, Player>,
	std::map<int, PlayerBehaviors>,
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
	std::map<int, LevelCompletionTimer>,
	std::map<int, Background>,
	std::map<int, Children>,
	std::map<int, Text>,
	std::map<int, ForceVisualization>,
	std::map<int, KeyConfigButton>,
	std::map<int, PauseMenuItems>,
	std::map<int, LevelButton>,
	std::map<int, Button>>
	ComponentMap;

class EntityBoundDrawable
{
public:
	int entity_id;
	sf::Drawable* drawable;
};

class Level
{
private:
	std::string savefile_path_;

	static int next_available_entity_id_;
	std::map<std::type_index, ComponentMap> components_;

public:
	const int id;
	bool editable = false;
	std::string name = "Untitled";
	sf::Vector2f size = sf::Vector2f(LEVEL_WIDTHS[2], LEVEL_WIDTHS[2] / ASPECT_RATIO);

	std::map<int, std::vector<EntityBoundDrawable>> drawables; // Indexed by draw priority

	template <class Component>
	std::map<int, Component>& GetComponent();

	template <class... Component>
	std::vector<std::tuple<int, Component*...>> GetEntitiesWith();

	Level(int level_id);
	int CreateEntityId();
	int CopyEntity(int from_id);
	void DeleteEntity(int id);
	int AddLevelButton(int level, std::function<void(void)> on_click, float pos_x, float pos_y, float width, float height, std::string image_path);
	int AddMenuButton(std::function<void(void)> on_click, float pos_x, float pos_y, float width, float height, std::string button_text, unsigned int text_size);
	int AddOptionsButton(sf::Keyboard::Key* key, float pos_x, float pos_y, float width, float height, std::string button_text, unsigned int text_size);

	void SaveToFile();
	void LoadFromFile();
	void SaveToFile(std::string savefile_path);
	void LoadFromFile(std::string savefile_path);
	int AddBlueprint(std::string tag);
};

#include "level.tpp"