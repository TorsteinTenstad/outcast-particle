#include "game.hpp"
#include "components/level_menu.hpp"
#include "systems/_pure_DO_systems.hpp"
#include "systems/coin.hpp"
#include "systems/draw.hpp"
#include "systems/level_completion_time.hpp"
#include "systems/level_menu.hpp"
#include "systems/menu_escape.hpp"
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

Game::Game()
{
	RegisterGameSystem<LevelReadyScreenSystem>();
	RegisterGameSystem<PlayerSystem>();
	RegisterGameSystem<SoundSystem>();
	RegisterGameSystem<EditModeUISystem>();
	RegisterGameSystem<MenuEscapeSystem>().Give(std::bind(&Game::GoToLastMenu, this)); //Must be above button system
	RegisterGameSystem<LevelMenuSystem>().Give(&level_groups_, &level_completion_time_records_, &level_coin_records_, std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::GenerateLevelTexture, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	RegisterGameSystem<ButtonSystem>();
	RegisterGameSystem<ScrollSystem>();		   // Has timing interactions with LevelMenuSystem and ButtonEventsSystem
	RegisterGameSystem<MenuNavigatonSystem>(); // Must be directly above ButtonEventsSystem for Hovered component to work correctly
	RegisterGameSystem<ButtonEventsSystem>();
	RegisterGameSystem<StickyButtonSystem>();
	RegisterGameSystem<SetDrawInfoSystem>();
	RegisterGameSystem<TrailSystem>();
	RegisterGameSystem<BackgroundSystem>(); // Must
	RegisterGameSystem<LevelCompletionTimeSystem>().SetLevelCompletionTimeRecords(&level_completion_time_records_);
	RegisterGameSystem<AnimatedPropertiesSystem>();
	RegisterGameSystem<FaceSystem>(); //Must be below AnimatedPropertiesSystem
	RegisterGameSystem<RenderGridAdaptiveTexturesSystem>();
	RegisterGameSystem<RenderTrailSystem>();
	RegisterGameSystem<RenderShapesSystem>();
	RegisterGameSystem<RenderTextSystem>();
	RegisterGameSystem<ForceVisualizationSystem>();
	RegisterGameSystem<SegmentedGlowEffectSystem>();
	RegisterGameSystem<ViewSystem>();
	RegisterGameSystem<DrawSystem>();
	RegisterGameSystem<EditModeSystem>();
	RegisterGameSystem<PauseMode>().Give(std::bind(&Game::SetLevel, this, std::placeholders::_1), &level_groups_);
	RegisterGameSystem<ScheduledDeleteSystem>();
	RegisterGameSystem<TextPopupSystem>();
	RegisterGameSystem<IntersectionSystem>();
	RegisterGameSystem<CollisionSystem>();
	RegisterGameSystem<GoalSystem>();
	RegisterGameSystem<KillOnIntersectionSystem>();
	RegisterGameSystem<CoinSystem>().SetCoinRecords(&level_coin_records_);

	RegisterPhysicsGameSystem<ElectricForceSystem>();
	RegisterPhysicsGameSystem<ElectricFieldForceSystem>();
	RegisterPhysicsGameSystem<MagneticFieldForceSystem>();
	RegisterPhysicsGameSystem<ForceSystem>();
	RegisterPhysicsGameSystem<AccelerationSystem>();
	RegisterPhysicsGameSystem<VelocitySystem>();

	const std::filesystem::path levels_path { "levels" };
	for (const auto& folder : std::filesystem::directory_iterator { levels_path })
	{
		for (const auto& level_file_path : std::filesystem::directory_iterator { folder.path() })
		{
			std::string level_id = level_file_path.path().string();
			std::string group = GetGroupNameFromId(level_id);
			level_groups_[group].push_back(level_id);
		}
	}

	if (!std::filesystem::exists("user"))
	{
		std::filesystem::create_directory("user");
	}

	LoadMapOfMapFromFile("user\\records.txt", level_completion_time_records_);
	LoadOptionsFromFile("user\\controls_config.txt", "user\\general_config.txt");

	CheckFullscreen();

	GoToMainMenu();
}

Game::~Game()
{
	SaveMapOfMapToFile("user\\records.txt", level_completion_time_records_);
	SaveOptionsToFile("user\\controls_config.txt", "user\\general_config.txt");
}

Level& Game::SetLevel(std::string level_id)
{
	assert(active_level_.GetMode() == PAUSE_MODE || IsMenu(active_level_id_));
	if (!IsMenu(level_id))
	{
		LevelMenuUI::last_at_level_id = level_id;
	}
	active_level_ = Level();
	bool level_id_is_top = (!menu_stack.empty() && level_id == menu_stack.top());
	if (IsMenu(active_level_id_) && !level_id_is_top)
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
		SaveMapOfMapToFile("user\\records.txt", level_completion_time_records_);
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
	else
	{
		active_level_.LoadFromFile(level_id);
		if (globals.general_config.use_ready_mode)
		{
			active_level_.SetMode(READY_MODE);
		}
	}
	globals.time_of_last_level_enter = globals.time;
	active_level_id_ = level_id;
	restart_update_loop_ = true;
	return active_level_;
}

void Game::Update(float dt)
{
	globals.time = absolute_clock.getElapsedTime().asSeconds();
	if (dt > 1.f / 30)
	{
		std::cout << "Lag spike detected, overriding dt\n";
		dt = 1.f / 30;
	}
	sfml_event_handler_.Update(cursor_and_keys_);
	for (const auto& system_id : game_system_ids_)
	{
		game_systems_.at(system_id)->Update(active_level_, dt);
		if (restart_update_loop_)
		{
			restart_update_loop_ = false;
			return;
		}
	}
	if (active_level_.GetMode() == PLAY_MODE)
	{
		for (int i = 0; i < physics_ticks_per_frame_; ++i)
		{
			for (const auto& system_id : physics_game_system_ids_)
			{
				game_systems_[system_id]->Update(active_level_, dt / physics_ticks_per_frame_);
				if (restart_update_loop_)
				{
					restart_update_loop_ = false;
					return;
				}
			}
			cursor_and_keys_.ResetFrameEvents();
		}
	}
}

std::string Game::GenerateLevelTexture(std::string level_id, unsigned width, unsigned height)
{
	std::string identifier = "_level_" + level_id;
	sf::Texture* texture = GetGameSystem<RenderShapesSystem>().RegisterTexture(identifier);
	Level level = Level();
	level.LoadFromFile(level_id);
	GetGameSystem<BackgroundSystem>().Update(level, 0);
	GetGameSystem<SetDrawInfoSystem>().Update(level, 0);
	GetGameSystem<SegmentedGlowEffectSystem>().Update(level, 0);
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
		globals.render_window.create(sf::VideoMode(1280, 720), "Volatile Particle", sf::Style::Default);
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

void Game::ExitGame()
{
	globals.render_window.close();
}