#include "game.hpp"
#include <chrono>
#include <filesystem>
#include <string>
#include <thread>

Game::Game()
{
	RegisterGameSystem<ModeSystem>();
	RegisterGameSystem<PlayerSystem>();
	RegisterGameSystem<SoundSystem>();
	RegisterGameSystem<ButtonSystem>();
	RegisterGameSystem<MouseInterationSystem>();
	RegisterGameSystem<SetDrawInfoSystem>();
	RegisterGameSystem<TrailSystem>();
	RegisterGameSystem<BackgroundSystem>();
	RegisterGameSystem<LevelCompletionTimeSystem>().SetLevelCompletionTimeRecords(&level_completion_time_records_);
	RegisterGameSystem<LevelButtonSystem>().SetLevelCompletionTimeRecords(&level_completion_time_records_);
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
	RegisterGameSystem<PauseMode>();
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

	const std::filesystem::path levels_path { "levels/" };
	for (const auto& folder : std::filesystem::directory_iterator { levels_path })
	{
		std::string group = SplitString(folder.path().stem().string(), "_").back();
		for (const auto& level_file_path : std::filesystem::directory_iterator { folder.path() })
		{
			int level_id = next_available_level_id_++;
			level_groups_[group].push_back(level_id);
			level_paths_[level_id] = level_file_path.path().string();
		}
	}

	GoToMainMenu();
}

void Game::SetLevel(int level_id)
{
	assert(level_id >= 0);
	assert(level_id < next_available_level_id_);
	active_level_.LoadFromFile(level_paths_[level_id]);
	active_level_id_ = level_id;
}

void Game::Update(float dt)
{
	sfml_event_handler_.Update(cursor_and_keys_);
	for (const auto& system_id : game_system_ids_)
	{
		game_systems_[system_id]->Update(active_level_, dt);
	}
	if (active_mode_ == PLAY_MODE)
	{
		for (int i = 0; i < physics_ticks_per_frame_; ++i)
		{
			for (const auto& system_id : physics_game_system_ids_)
			{
				game_systems_[system_id]->Update(active_level_, dt / physics_ticks_per_frame_);
			}
			cursor_and_keys_.ResetFrameEvents();
		}
	}
}

std::string Game::GenerateLevelTexture(int level_id)
{
	sf::Texture texture;
	texture.create(globals.render_window.getSize().x, globals.render_window.getSize().y);
	int active_level_before_capture = active_level_id_;
	SetLevel(level_id);
	globals.render_window.setView(sf::View(active_level_.size / 2.f, active_level_.size));
	Update(0);
	SetLevel(active_level_before_capture);
	texture.update(globals.render_window);
	std::string identifier = "level" + std::to_string(level_id);
	GetGameSystem<RenderShapesSystem>().RegisterTexture(identifier, texture);
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

void Game::ResetActiveLevel()
{
}
