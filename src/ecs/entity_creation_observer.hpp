#pragma once
#include "ecs/ecs_scene.hpp"
#include <functional>

class ECSScene;

class EntityCreationObserver
{
private:
	std::function<void(ECSScene&, int)> f_;
	ECSScene& subject_;

public:
	EntityCreationObserver(ECSScene& subject, std::function<void(ECSScene&, int)> f);
	~EntityCreationObserver();
	void Notify(int created_id);
};