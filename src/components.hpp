#pragma once
#include "components/animated_position.hpp"
#include "components/area.hpp"
#include "components/blueprint_menu_item.hpp"
#include "components/button.hpp"
#include "components/button_events.hpp"
#include "components/children.hpp"
#include "components/coin.hpp"
#include "components/collision.hpp"
#include "components/draw_info.hpp"
#include "components/edit_mode.hpp"
#include "components/editable.hpp"
#include "components/force_visualization.hpp"
#include "components/goal.hpp"
#include "components/grid_adaptive_textures.hpp"
#include "components/intersection.hpp"
#include "components/kill_on_intersection.hpp"
#include "components/level_menu.hpp"
#include "components/menu_navigator.hpp"
#include "components/not_serialized.hpp"
#include "components/pause_menu_items.hpp"
#include "components/physics.hpp"
#include "components/player.hpp"
#include "components/scheduled_delete.hpp"
#include "components/scroll.hpp"
#include "components/shader.hpp"
#include "components/sound_info.hpp"
#include "components/sticky_button.hpp"
#include "components/tag.hpp"
#include "components/text.hpp"
#include "components/text_popup.hpp"
#include "components/timers.hpp"
#include "components/trail.hpp"

#include <variant>

typedef std::variant<
	std::map<int, Acceleration>,
	std::map<int, AnimatedPosition>,
	std::map<int, Background>,
	std::map<int, BinaryOptionsButton>,
	std::map<int, BlueprintMenuItem>,
	std::map<int, Charge>,
	std::map<int, ChargeDependentDrawInfo>,
	std::map<int, Children>,
	std::map<int, Coin>,
	std::map<int, FillColor>,
	std::map<int, CoinCounter>,
	std::map<int, Collision>,
	std::map<int, DrawInfo>,
	std::map<int, DrawPriority>,
	std::map<int, Editable>,
	std::map<int, EditMode>,
	std::map<int, ElectricField>,
	std::map<int, Face>,
	std::map<int, ForceVisualization>,
	std::map<int, Goal>,
	std::map<int, Intersection>,
	std::map<int, KeyConfigButton>,
	std::map<int, KillOnIntersection>,
	std::map<int, Laser>,
	std::map<int, LaserTexture>,
	std::map<int, LevelCompletionTimer>,
	std::map<int, LevelMenuUI>,
	std::map<int, MagneticField>,
	std::map<int, MenuNavigator>,
	std::map<int, MenuNavigatable>,
	std::map<int, NotSerialized>,
	std::map<int, Hovered>,
	std::map<int, OnHovered>,
	std::map<int, HoveredStartedThisFrame>,
	std::map<int, OnHoveredStartedThisFrame>,
	std::map<int, OnReleasedThisFrame>,
	std::map<int, OrientationDependentDrawInfo>,
	std::map<int, PauseMenuItems>,
	std::map<int, Player>,
	std::map<int, PlayerBehaviors>,
	std::map<int, Position>,
	std::map<int, Pressed>,
	std::map<int, PressedThisFrame>,
	std::map<int, Radius>,
	std::map<int, ReceivedForces>,
	std::map<int, ReceivesButtonEvents>,
	std::map<int, MouseInteractionDependentFillColor>,
	std::map<int, ReleasedThisFrame>,
	std::map<int, ScheduledDelete>,
	std::map<int, ScrollWindow>,
	std::map<int, ShortcutKey>,
	std::map<int, SegmentedGlowEffect>,
	std::map<int, Selected>,
	std::map<int, Shader>,
	std::map<int, SliderButton>,
	std::map<int, StickyButton>,
	std::map<int, StickyButtonDown>,
	std::map<int, MenuDelayTimer>,
	std::map<int, SoundInfo>,
	std::map<int, Tag>,
	std::map<int, Text>,
	std::map<int, TextPopup>,
	std::map<int, TextPopupSpawner>,
	std::map<int, Trail>,
	std::map<int, Velocity>,
	std::map<int, Wall>,
	std::map<int, WallTexture>,
	std::map<int, WidthAndHeight>>
	ComponentMap;