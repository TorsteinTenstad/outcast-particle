#pragma once
#include "blueprint.hpp"
#include "components/shader.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "ecs/ecs_scene_with_inactive_layer.hpp"
#include "error.hpp"
#include "globals.hpp"
#include "level_mode.hpp"
#include "ui_origin.hpp"
#include "undo_system.hpp"
#include <array>
#include <functional>
#include <optional>
#include <variant>

class EntityBoundDrawable
{
public:
	sf::Drawable* drawable;
	std::optional<Entity> entity;
	std::optional<Shader*> shader;
};

const std::array<sf::Vector2u, 5> LEVEL_SIZES { { sf::Vector2u(16, 9), sf::Vector2u(32, 18), sf::Vector2u(48, 27), sf::Vector2u(64, 36), sf::Vector2u(80, 45) } };

#define DEFAULT_LEVEL_GRID_SIZE_ID 1

class Level : public ECSSceneWithInactiveLayer
{
private:
	std::string savefile_path_;
	LevelMode mode_ = PLAY_MODE;
	int grid_size_id = DEFAULT_LEVEL_GRID_SIZE_ID;

	EntityCreationObserver entity_creation_observer_;

public:
	Level(const Level&) = delete;
	Level();
	bool editable = false;
	std::string name = "Untitled";
	std::string music_path = "content\\music\\bliss.wav";
	sf::Vector2f ui_bars_size = sf::Vector2f(0, 0);

	UndoSystem editor;

	std::map<int, std::vector<EntityBoundDrawable>> drawables; // Indexed by draw priority

	Entity AddBlueprint(Blueprint blueprint);
	Entity AddBlueprint(std::string blueprint_tag);

	LevelState ComputeState();
	LevelMode GetMode();
	void SetMode(LevelMode level_mode);

	sf::Vector2f GetSize();
	sf::Vector2u GetGridSize();
	float GetScale();
	void ResetSize();
	int GetValidNewSizeId(int increment);
	void ModifyLevelSize(int increment);

	void SaveToFile();
	Error LoadFromFile();
	void SaveToFile(std::string savefile_path);
	Error LoadFromFile(std::string savefile_path);
	void DiscardChanges();
};
