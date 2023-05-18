#pragma once
#include "ecs/ecs_scene.hpp"
#include <functional>

class ECSScene;

class EntityCreationObserver
{
private:
	std::function<void(ECSScene&, Entity)> f_;
	ECSScene& subject_;

public:
	EntityCreationObserver(ECSScene& subject, std::function<void(ECSScene&, Entity)> f);
	~EntityCreationObserver();
	void Notify(Entity created_id);
};