#include "game.hpp"
#include "userdata_storage.hpp"
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
	RegisterGameSystem<ButtonEventsSystem>();
	RegisterGameSystem<ScrollSystem>();
	RegisterGameSystem<StickyButtonSystem>();
	RegisterGameSystem<MenuNavigatonSystem>(); // Must be directly below ButtonEventsSystem for Hovered component to work correctly
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
	RegisterGameSystem<PauseMode>().Give(std::bind(&Game::SetLevel, this, std::placeholders::_1), &level_groups_);
	RegisterGameSystem<MenuEscapeSystem>().Give(std::bind(&Game::GoToLastMenu, this));
	RegisterGameSystem<ScheduledDeleteSystem>();
	RegisterGameSystem<TextPopupSystem>();
	RegisterGameSystem<AnimatedPositionSystem>();
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

	//LoadMapFromFile("user\\coin_record.txt", level_coin_records_);
	//LoadMapFromFile("user\\completion_time_record.txt", level_completion_time_records_);
	LoadMapOfMapFromFile("user\\records.txt", level_completion_time_records_);
	LoadOptionsFromFile("user\\controls_config.txt");

	GoToMainMenu();
}

Game::~Game()
{
	//SaveMapToFile("user\\coin_record.txt", level_coin_records_);
	//SaveMapToFile("user\\completion_time_record.txt", level_completion_time_records_);
	SaveMapOfMapToFile("user\\records.txt", level_completion_time_records_);
	SaveOptionsToFile("user\\controls_config.txt");
}

Level& Game::SetLevel(std::string level_id)
{
	assert(active_level_.GetMode() == PAUSE_MODE || IsMenu(active_level_id_));
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
	else
	{
		active_level_.LoadFromFile(level_id);
	}
	active_level_id_ = level_id;
	restart_update_loop_ = true;
	return active_level_;
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