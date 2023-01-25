#include "game.hpp"
#include <chrono>
#include <filesystem>
#include <functional>
#include <string>
#include <thread>

Game::Game()
{
	RegisterGameSystem<PlayerSystem>();
	RegisterGameSystem<SoundSystem>();
	RegisterGameSystem<ButtonSystem>();
	RegisterGameSystem<LevelMenuSystem>().Give(&level_groups_, &level_completion_time_records_, &level_coin_records_, std::bind(&Game::SetLevel, this, std::placeholders::_1), std::bind(&Game::GenerateLevelTexture, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	RegisterGameSystem<MouseInterationSystem>();
	RegisterGameSystem<MenuNavigatonSystem>(); // Must be directly below MouseInterationSystem for Hovered component to work correctly
	RegisterGameSystem<SetDrawInfoSystem>();
	RegisterGameSystem<TrailSystem>();
	RegisterGameSystem<BackgroundSystem>();
	RegisterGameSystem<LevelCompletionTimeSystem>().SetLevelCompletionTimeRecords(&level_completion_time_records_);
	RegisterGameSystem<FaceSystem>();
	RegisterGameSystem<RenderTrailSystem>();
	RegisterGameSystem<RenderShapesSystem>();
	RegisterGameSystem<RenderTextSystem>();
	RegisterGameSystem<ForceVisualizationSystem>();
	RegisterGameSystem<SegmentedGlowEffectSystem>();
	RegisterGameSystem<DrawSystem>();
	RegisterGameSystem<DisplayVelocitySystem>();
	RegisterGameSystem<EditModeSystem>();
	RegisterGameSystem<ViewSystem>();
	RegisterGameSystem<PauseMode>().GiveFunctions(std::bind(&Game::SetLevel, this, std::placeholders::_1));
	RegisterGameSystem<ScheduledDeleteSystem>();

	RegisterPhysicsGameSystem<ElectricForceSystem>();
	RegisterPhysicsGameSystem<ElectricFieldForceSystem>();
	RegisterPhysicsGameSystem<MagneticFieldForceSystem>();
	RegisterPhysicsGameSystem<ForceSystem>();
	RegisterPhysicsGameSystem<AccelerationSystem>();
	RegisterPhysicsGameSystem<VelocitySystem>();
	RegisterPhysicsGameSystem<TextPopupSystem>();
	RegisterPhysicsGameSystem<AnimatedPositionSystem>();
	RegisterPhysicsGameSystem<IntersectionSystem>();
	RegisterPhysicsGameSystem<CollisionSystem>();
	RegisterPhysicsGameSystem<GoalSystem>();
	RegisterPhysicsGameSystem<KillOnIntersectionSystem>();
	RegisterPhysicsGameSystem<CoinSystem>().SetCoinRecords(&level_coin_records_);

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

	GoToMainMenu();
}

void Game::SetLevel(std::string level_id)
{
	assert(active_level_.GetMode() == PAUSE_MODE || IsMenu(active_level_id_));
	active_level_ = Level();
	if (level_id == MAIN_MENU)
	{
		GoToMainMenu();
	}
	else if (level_id == LEVEL_MENU)
	{
		GoToLevelMenu();
	}
	else if (level_id == OPTIONS_MENU)
	{
		GoToOptionsMenu();
	}
	else
	{
		active_level_.LoadFromFile(level_id);
		if (is_in_level_editing_)
		{
			active_level_.SetMode(EDIT_MODE);
		}
	}
	active_level_id_ = level_id;
	restart_update_loop_ = true;
}

void Game::Update(float dt)
{
	sfml_event_handler_.Update(cursor_and_keys_);
	for (const auto& system_id : game_system_ids_)
	{
		game_systems_[system_id]->Update(active_level_, dt);
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
	GetGameSystem<PlayerSystem>().Update(level, 0);
	GetGameSystem<FaceSystem>().Update(level, 0);
	GetGameSystem<RenderShapesSystem>().Update(level, 0);
	GetGameSystem<RenderTextSystem>().Update(level, 0);
	GetGameSystem<DrawSystem>().CaptureLevel(level, texture, width, height);
	return identifier;
}

void Game::ToggleFullscreen()
{
	if (fullscreen_)
	{
		globals.render_window.create(sf::VideoMode(1280, 720), "outcast-particle");
	}
	else
	{
		globals.render_window.create(sf::VideoMode::getFullscreenModes()[0], "outcast-particle", sf::Style::Fullscreen);
	}
	fullscreen_ = !fullscreen_;
}

void Game::ExitGame()
{
	globals.render_window.close();
}