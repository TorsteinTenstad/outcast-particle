#pragma once
#include "ecs/non_copyable.hpp"

class CreationData : NonCopyableInternally
{
public:
	float creation_time = -1;
};