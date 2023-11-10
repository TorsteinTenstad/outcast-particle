#include "game.hpp"
#include "controls_config.hpp"
#include "entity_creation.hpp"
#include "folder_definitions.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "systems/draw.hpp"
#include "systems/level_completion_time.hpp"
#include "systems/level_menu.hpp"
#include "systems/menu_escape.hpp"
#include "systems/music.hpp"
#include "systems/pause_mode.hpp"
#include "systems/render_grid_adaptive_textures.hpp"
#include "systems/render_shapes.hpp"
#include "systems/render_text.hpp"
#include "systems/render_trail.hpp"
#include "systems/sound_system.hpp"
#include "userdata_storage.hpp"
#include "utils/level_id.hpp"
#include <chrono>
#include <filesystem>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

Game::Game() :
	active_level_(std::make_unique<Level>()),
	level_manager_(LEVELS_DIR),
	records_(USER_DIR / "records.txt"),
	steam_leaderboards_(records_)
{
	//RegisterGameSystem<TrailerIntroHelperSystem>();
	RegisterGameSystem<LevelReadyScreenSystem>();
	RegisterGameSystem<PlayerSystem>();
	RegisterGameSystem<SoundSystem>();
	RegisterGameSystem<MusicSystem>();
	RegisterGameSystem<MenuEscapeSystem>().Give(std::bind(&Game::GoToLastMenu, this)); //Must be above button system
	RegisterGameSystem<LevelMenuSystem>().Give(&level_manager_, &records_, std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::SetLevelAndEdit, this, std::placeholders::_1), std::bind(&Game::GenerateLevelTexture, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	RegisterGameSystem<OpenFileDialogSystem>();
	RegisterGameSystem<EditModeUISystem>(); // Should be above ButtonSystem for edit mode menus to not flash white on creation
	RegisterGameSystem<ButtonSystem>();
	RegisterGameSystem<ScrollSystem>();		   // Has timing interactions with LevelMenuSystem and ButtonEventsSystem
	RegisterGameSystem<MenuNavigatorSystem>(); // Must be directly above ButtonEventsSystem for Hovered component to work correctly // Consumes button events
	RegisterGameSystem<ButtonEventsSystem>();
	RegisterGameSystem<StickyButtonSystem>();
	RegisterGameSystem<CanDisableButtonSystem>();
	RegisterGameSystem<ExitGameSystem>(); //Should be close after ButtonSystem for exiting to go smoothly.
	RegisterGameSystem<TooltipSystem>();
	RegisterGameSystem<ScaleWithLevelSystem>();
	RegisterGameSystem<SetDrawInfoSystem>();
	RegisterGameSystem<TrailSystem>();
	RegisterGameSystem<LevelCompletionTimeSystem>().SetLevelCompletionTimeRecords(&records_);
	RegisterGameSystem<WormholeSystem>();
	RegisterGameSystem<AnimatedPropertiesSystem>();
	RegisterGameSystem<FaceSystem>(); //Must be below AnimatedPropertiesSystem
	RegisterGameSystem<TextBoxSystem>();
	RegisterGameSystem<TogglingLaserSystem>();
	RegisterGameSystem<AddKillOnIntersectionSystem>(); // Should be directly below TogglingLaserSystem for
	RegisterGameSystem<ViewSystem>();
	RegisterGameSystem<BackgroundSystem>(); // Must be after ViewSystem for background to update to fit IU bar
	RegisterGameSystem<RenderGridAdaptiveTexturesSystem>();
	RegisterGameSystem<RenderTrailSystem>();
	RegisterGameSystem<RenderShapesSystem>();
	RegisterGameSystem<RenderTextSystem>();
	RegisterGameSystem<ForceVisualizationSystem>();
	RegisterGameSystem<DrawSystem>();
	RegisterGameSystem<EditModeSystem>();
	RegisterGameSystem<ScreenWideShaderEffectsSystem>(); // Must be below EditModeSystem for effects to be visible on copy
	RegisterGameSystem<EditModeSelectedEffectSystem>();
	RegisterGameSystem<PauseMode>().Give(std::bind(&Game::SetLevel, this, std::placeholders::_1), &level_manager_.GetLevels(), &records_);
	RegisterGameSystem<ScheduledDeleteSystem>();
	RegisterGameSystem<TextPopupSystem>();
	RegisterGameSystem<SetWallSoundSystem>();
	RegisterGameSystem<LaserProximitySystem>();
	RegisterGameSystem<ShowErrorSystem>();

	RegisterPhysicsGameSystem<IntersectionSystem>();
	RegisterPhysicsGameSystem<CollisionSystem>();
	RegisterPhysicsGameSystem<GoalSystem>();
	RegisterPhysicsGameSystem<TimerSystem>();
	RegisterPhysicsGameSystem<KillOnIntersectionSystem>();
	RegisterPhysicsGameSystem<CoinSystem>();
	RegisterPhysicsGameSystem<ElectricForceSystem>();
	RegisterPhysicsGameSystem<ElectricFieldForceSystem>();
	RegisterPhysicsGameSystem<MagneticFieldForceSystem>();
	RegisterPhysicsGameSystem<ForceSystem>();
	RegisterPhysicsGameSystem<AccelerationSystem>();
	RegisterPhysicsGameSystem<VelocitySystem>();

	if (!std::filesystem::exists(USER_DIR))
	{
		std::filesystem::create_directory(USER_DIR);
	}

	LoadOptionsFromFile((USER_DIR / "controls_config.txt").string(), (USER_DIR / "general_config.txt").string());

	CheckFullscreen();

	GoToMainMenu();
}

Game::~Game()
{
	SaveOptionsToFile((USER_DIR / "controls_config.txt").string(), (USER_DIR / "general_config.txt").string());
}

Level& Game::SetLevelAndEdit(std::string level_id)
{
	Level& new_level = SetLevel(level_id);
	new_level.SetMode(EDIT_MODE);
	is_in_level_editing_ = true;
	return new_level;
}

Level& Game::SetLevel(std::string level_id)
{
	assert(active_level_->GetMode() == PAUSE_MODE || IsMenu(active_level_id_));
	active_level_ = std::move(std::make_unique<Level>());
	bool level_id_is_top = (!menu_stack.empty() && level_id == menu_stack.top());
	if (IsMenu(active_level_id_) && !level_id_is_top && active_level_id_ != level_id)
	{
		menu_stack.push(active_level_id_);
	}
	if (level_id_is_top)
	{
		menu_stack.pop();
	}

	if (level_id == MAIN_MENU)
	{
		GoToMainMenu();
		menu_stack = {};
	}
	else if (level_id == LEVEL_MENU)
	{
		GoToLevelMenu();
	}
	else if (level_id == OPTIONS_MENU)
	{
		GoToOptionsMenu();
	}
	else if (level_id == KEY_CONFIG_MENU)
	{
		GoToKeyConfigMenu();
	}
	else if (level_id == GRAPHICS_AND_DISPLAY_MENU)
	{
		GoToGraphicsAndDisplayMenu();
	}
	else if (level_id == MUSIC_AND_SOUND_MENU)
	{
		GoToMusicAndSoundMenu();
	}
	else if (level_id == CREDITS_MENU)
	{
		GoToCreditsMenu();
	}
	else
	{
		active_level_->LoadFromFile(level_id);
		active_level_->SetMode(READY_MODE);
	}
	active_level_id_ = level_id;
	globals.skip_drawing_this_frame = true;
	return *active_level_;
}

void Game::Update(float dt)
{
	if (dt > 1.f / 20)
	{
		//std::cout << "Lag spike detected, overriding dt\n";
		dt = 1.f / 20;
	}
	globals.time += dt;
	sfml_event_handler_.Update(cursor_and_keys_);
	if (cursor_and_keys_.key_released_this_frame[sf::Keyboard::F11])
	{
		ToggleFullscreen();
	}
	steam_leaderboards_.Update();

	for (const auto& system_id : game_system_entities_)
	{
		game_systems_.at(system_id)->Update(*active_level_, dt);
	}
	if (active_level_->GetMode() == PLAY_MODE)
	{
		for (int i = 0; i < physics_ticks_per_frame_; ++i)
		{
			for (const auto& system_id : physics_game_system_entities_)
			{
				game_systems_[system_id]->Update(*active_level_, dt / physics_ticks_per_frame_);
			}
			cursor_and_keys_.ResetFrameEvents();
		}
	}
}

std::string Game::GenerateLevelTexture(std::string level_id, unsigned width, unsigned height)
{
	std::string identifier = "_level_" + level_id;
	sf::Texture* texture = GetGameSystem<RenderShapesSystem>().RegisterTexture(identifier);
	Level level;
	level.LoadFromFile(level_id);
	GetGameSystem<BackgroundSystem>().Update(level, 0);
	GetGameSystem<TextPopupSystem>().Update(level, 0);
	GetGameSystem<SetDrawInfoSystem>().Update(level, 0);
	GetGameSystem<ScreenWideShaderEffectsSystem>().Update(level, 0);
	GetGameSystem<PlayerSystem>().Update(level, 0);
	GetGameSystem<FaceSystem>().Update(level, 0);
	GetGameSystem<RenderGridAdaptiveTexturesSystem>().Update(level, 0);
	GetGameSystem<RenderShapesSystem>().Update(level, 0);
	GetGameSystem<RenderTextSystem>().Update(level, 0);
	GetGameSystem<DrawSystem>().CaptureLevel(level, texture, width, height);
	return identifier;
}

void Game::CheckFullscreen()
{
	if (globals.general_config.fullscreen)
	{
		globals.render_window.create(sf::VideoMode::getFullscreenModes()[0], "Volatile Particle", sf::Style::Fullscreen);
	}
	else
	{
		globals.render_window.create(sf::VideoMode(1920, 1080), "Volatile Particle", sf::Style::Default);
	}
	globals.render_window.setVerticalSyncEnabled(true);
}

void Game::ToggleFullscreen()
{
	globals.general_config.fullscreen = !globals.general_config.fullscreen;
	CheckFullscreen();
}

void Game::GoToLastMenu()
{

	if (menu_stack.empty())
	{
		assert(active_level_id_ == MAIN_MENU);
		return;
	}
	SetLevel(menu_stack.top());
}

void Game::ResetKeyConfig()
{
	KeyConfig key_config;
	globals.key_config = key_config;
	SetLevel(KEY_CONFIG_MENU);
}

void Game::ExitGame()
{
	globals.render_window.close();
}