#pragma once
#include "ecs_scene.hpp"
#include <functional>

class ESCScene;

class EntityCreationObserver
{
private:
	std::function<void(ESCScene&, int)> f_;
	ESCScene& subject_;

public:
	EntityCreationObserver(ESCScene& subject, std::function<void(ESCScene&, int)> f);
	~EntityCreationObserver();
	void Notify(int created_id);
};