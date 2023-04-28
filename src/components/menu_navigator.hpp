#pragma once
#include <optional>

class MenuNavigator
{
public:
	std::optional<int> currently_at_entity_id;
	bool moved_itself_this_frame = false;
};

class MenuNavigable
{
};