#pragma once

#include "game_system.hpp"
#include "level.hpp"

#define CREATE_PURE_DO_SYSTEM_DECLARATION(SYSTEM_NAME) \
	class SYSTEM_NAME : public GameSystem              \
	{                                                  \
	public:                                            \
		using GameSystem::GameSystem;                  \
		void Update(Level& level, float dt);           \
	};

CREATE_PURE_DO_SYSTEM_DECLARATION(AccelerationSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(AnimatedPropertiesSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(BackgroundSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ButtonEventsSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ButtonSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(CollisionSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(EditModeSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(EditModeSelectedEffectSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(EditModeUISystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ElectricFieldForceSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ElectricForceSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(FaceSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ForceVisualizationSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ForceSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(GoalSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(IntersectionSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(LaserProximitySystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(KillOnIntersectionSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(LevelReadyScreenSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(MagneticFieldForceSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(MenuNavigatorSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(PlayerSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ScheduledDeleteSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ScrollSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ScreenWideShaderEffectsSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(SetDrawInfoSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(StickyButtonSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(TextBoxSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(TextPopupSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(TimerSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(TrailSystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(VelocitySystem);
CREATE_PURE_DO_SYSTEM_DECLARATION(ViewSystem);