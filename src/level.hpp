#pragma once
#include "components/animated_position.hpp"
#include "components/area.hpp"
#include "components/blueprint_menu_item.hpp"
#include "components/button.hpp"
#include "components/children.hpp"
#include "components/coin.hpp"
#include "components/collision.hpp"
#include "components/draw_info.hpp"
#include "components/editable.hpp"
#include "components/force_visualization.hpp"
#include "components/goal.hpp"
#include "components/intersection.hpp"
#include "components/kill_on_intersection.hpp"
#include "components/level_button.hpp"
#include "components/level_completion_timer.hpp"
#include "components/menu_navigator.hpp"
#include "components/pause_menu_items.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/player_behaviours.hpp"
#include "components/pressed.hpp"
#include "components/scheduled_delete.hpp"
#include "components/shader.hpp"
#include "components/sound_info.hpp"
#include "components/tag.hpp"
#include "components/text.hpp"
#include "components/text_popup.hpp"
#include "components/trail.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "globals.hpp"
#include "level_mode.hpp"
#include "level_state.hpp"
#include <functional>
#include <map>
#include <typeindex>
#include <variant>

typedef std::variant<
	std::map<int, Acceleration>,
	std::map<int, AnimatedPosition>,
	std::map<int, Background>,
	std::map<int, BlueprintMenuItem>,
	std::map<int, Border>,
	std::map<int, Charge>,
	std::map<int, ChargeDependentDrawInfo>,
	std::map<int, Children>,
	std::map<int, Coin>,
	std::map<int, CoinCounter>,
	std::map<int, Collision>,
	std::map<int, DrawInfo>,
	std::map<int, DrawPriority>,
	std::map<int, Editable>,
	std::map<int, ElectricField>,
	std::map<int, Face>,
	std::map<int, ForceVisualization>,
	std::map<int, Goal>,
	std::map<int, Intersection>,
	std::map<int, KeyConfigButton>,
	std::map<int, KillOnIntersection>,
	std::map<int, LevelButton>,
	std::map<int, LevelCompletionTimer>,
	std::map<int, MagneticField>,
	std::map<int, MenuNavigator>,
	std::map<int, MenuNavigatable>,
	std::map<int, MouseEnteredThisFrame>,
	std::map<int, OnMouseEnteredThisFrame>,
	std::map<int, OnReleasedThisFrame>,
	std::map<int, OrientationDependentDrawInfo>,
	std::map<int, PauseMenuItems>,
	std::map<int, Player>,
	std::map<int, PlayerBehaviors>,
	std::map<int, Position>,
	std::map<int, Pressed>,
	std::map<int, PressedImagePath>,
	std::map<int, PressedThisFrame>,
	std::map<int, Radius>,
	std::map<int, ReceivedForces>,
	std::map<int, ReceivesMouseEvents>,
	std::map<int, ReleasedThisFrame>,
	std::map<int, ScheduledDelete>,
	std::map<int, SegmentedGlowEffect>,
	std::map<int, Selected>,
	std::map<int, Shader>,
	std::map<int, SoundInfo>,
	std::map<int, Tag>,
	std::map<int, Text>,
	std::map<int, TextPopup>,
	std::map<int, TextPopupSpawner>,
	std::map<int, Trail>,
	std::map<int, Velocity>,
	std::map<int, WidthAndHeight>>
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
	LevelMode mode_ = PLAY_MODE;

	static int next_available_entity_id_;
	std::map<std::type_index, ComponentMap> components_;

public:
	bool editable = false;
	std::string name = "Untitled";
	sf::Vector2f size = sf::Vector2f(LEVEL_WIDTHS[2], LEVEL_WIDTHS[2] / ASPECT_RATIO);

	std::map<int, std::vector<EntityBoundDrawable>> drawables; // Indexed by draw priority

	template <class Component>
	std::map<int, Component>& GetComponent();

	template <class... Component>
	std::vector<std::tuple<int, Component*...>> GetEntitiesWith();

	template <class... Component>
	void DeleteEntitiesWith();

	Level();
	int CreateEntityId();
	int AddBlueprint(std::string tag);

	int CopyEntity(int from_id);
	void DeleteEntity(int id);

	LevelState ComputeState();
	LevelMode GetMode();
	void SetMode(LevelMode level_mode);

	void SaveToFile();
	void LoadFromFile();
	void SaveToFile(std::string savefile_path);
	void LoadFromFile(std::string savefile_path);
};

int AddMenuButton(Level& level, std::function<void(void)> on_click, float pos_x, float pos_y, std::string button_text);
int AddOptionsButton(Level& level, sf::Keyboard::Key* key, float pos_x, float pos_y, float width, float height, std::string button_text, unsigned int text_size);
int CreateScreenwideFragmentShaderEntity(Level& level, std::string shader_path, int draw_priority);

#include "level.tpp"

template <class Component>
Component* GetSingleton(Level& level);

template <class ResponsibleComponent>
Shader* EnsureExistanceOfScreenwideFragmentShaderChildEntity(Level& level, Children* parents_children, std::string shader_path, int draw_priority);