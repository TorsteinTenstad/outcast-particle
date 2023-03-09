#pragma once
#include "blueprint.hpp"
#include "constants.hpp"
#include "cursor_and_keys.hpp"
#include "ecs_scene.hpp"
#include "globals.hpp"
#include "level_mode.hpp"
#include "ui_origin.hpp"
#include <functional>
#include <variant>

class EntityBoundDrawable
{
public:
	int entity_id;
	sf::Drawable* drawable;
};

class Level : public ESCScene
{
private:
	std::string savefile_path_;
	LevelMode mode_ = PLAY_MODE;
	int grid_size_id = DEFAULT_LEVEL_GRID_SIZE_ID;

public:
	bool editable = false;
	std::string name = "Untitled";
	sf::Vector2f ui_bars_size = sf::Vector2f(0, 0);

	std::map<int, std::vector<EntityBoundDrawable>> drawables; // Indexed by draw priority

	int AddBlueprint(Blueprint blueprint);

	LevelState ComputeState();
	LevelMode GetMode();
	void SetMode(LevelMode level_mode);

	sf::Vector2f GetSize();
	sf::Vector2u GetGridSize();
	float GetScale();
	void ResetSize();
	void IncreaseSize();
	void DecreaseSize();

	void
	SaveToFile();
	void LoadFromFile();
	void SaveToFile(std::string savefile_path);
	void LoadFromFile(std::string savefile_path);

	friend class CollisionSystem; // System is written using the old GetComponent. Gets access to the new, private version until the system is rewritten.
	friend class EditModeSystem;  // System is written using the old GetComponent. Gets access to the new, private version until the system is rewritten.
};
