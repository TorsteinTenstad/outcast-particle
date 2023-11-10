#pragma once
#include "ecs/entity.hpp"
#include <map>
#include <optional>
#include <string>
#include <vector>

class ReDrawLevelMenuRequest
{
public:
	std::optional<std::string> at_group;
	std::optional<std::string> at_level_id;
};

class LevelMenuUI
{
public:
	inline static std::map<std::string, std::string> last_at_level_id;
	inline static std::optional<std::string> last_at_level_group;

	std::optional<std::string> at_level_id;
	std::optional<std::string> at_group;

	std::optional<std::string> displaying_level_id;

	std::optional<Entity> new_level_button_id;
	std::optional<Entity> import_level_button_id;
	Entity menu_navigator_entity;
	Entity level_preview_entity;
	Entity next_group_button_entity;
	Entity prev_group_button_entity;
	Entity dot_indicator_entity;

	std::vector<Entity> button_entities = {};
	std::vector<Entity> text_entities = {};
	std::vector<Entity> rename_level_button_entities = {};
	std::vector<Entity> edit_level_button_entities = {};
	std::vector<Entity> delete_level_button_entities = {};

	std::vector<Entity> badge_entities = {};
	std::vector<Entity> record_block_entities = {};
};