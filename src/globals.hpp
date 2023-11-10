#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "controls_config.hpp"
#include "error.hpp"
#include "general_user_config.hpp"
#include <deque>
#include <optional>
#include <set>
#include <string>
#include <vector>

const std::vector<std::string> level_groups_accessible_in_trial = { "Lab 0", "Lab 1" };
const std::vector<std::string> editable_level_groups = { "Custom Levels" };

#ifdef _TRIAL_VERSION
constexpr bool IS_TRIAL_BUILD = true;
#else
constexpr bool IS_TRIAL_BUILD = false;
#endif

#ifdef _DEVELOPER_OPTIONS_ENABLED
constexpr bool DEVELOPER_OPTIONS_ENABLED = true;
#else
constexpr bool DEVELOPER_OPTIONS_ENABLED = false;
#endif

class ContentAccessOptions
{
private:
	bool all_level_groups_are_editable = false;
	bool is_trial_build = IS_TRIAL_BUILD;

public:
	void ToggleAllLevelsAreEditable();
	void ToggleTrialBuild();

	bool IsLevelGroupEditable(const std::string& group_display_name);
	bool IsLevelAccessible(const std::string& level_id);
};

struct Globals
{
	sf::RenderWindow render_window;
	float time = 0;
	bool skip_drawing_this_frame = false;
	KeyConfig key_config;
	GeneralConfig general_config;
	ContentAccessOptions content_access_options;
	Error errors;
	std::string steam_username;
	uint64_t steam_user_id;
};

extern Globals& globals;
