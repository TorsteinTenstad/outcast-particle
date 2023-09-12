#include "globals.hpp"
#include "utils/container_operations.hpp"
#include "utils/level_id.hpp"
#include <iostream>

void ContentAccessOptions::ToggleAllLevelsAreEditable()
{
	all_level_groups_are_editable = !all_level_groups_are_editable;
}
void ContentAccessOptions::ToggleTrialBuild()
{
	is_trial_build = !is_trial_build;
}

bool ContentAccessOptions::IsLevelGroupEditable(const std::string& group_name)
{
    std::string group_display_name = GetGroupDisplayNameFromGroupName(group_name);
	bool filtered_is_trial_build = DEVELOPER_OPTIONS_ENABLED ? is_trial_build : IS_TRIAL_BUILD;
	bool filtered_all_level_groups_are_editable = DEVELOPER_OPTIONS_ENABLED ? all_level_groups_are_editable : false;
	return !filtered_is_trial_build && (Contains(editable_level_groups, group_display_name) || filtered_all_level_groups_are_editable);
}
bool ContentAccessOptions::IsLevelAccessible(const std::string& level_id)
{
	bool filtered_is_trial_build = DEVELOPER_OPTIONS_ENABLED ? is_trial_build : IS_TRIAL_BUILD;
	return !filtered_is_trial_build || Contains(level_groups_accessible_in_trial, GetGroupDisplayNameFromId(level_id));
}